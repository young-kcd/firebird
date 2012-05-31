/*
 *	PROGRAM:		JRD access method
 *	MODULE:			CryptoManager.cpp
 *	DESCRIPTION:	Database encryption
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"
#include "firebird/Crypt.h"
#include "gen/iberror.h"
#include "../jrd/CryptoManager.h"

#include "../common/classes/alloc.h"
#include "../jrd/Database.h"
#include "../jrd/ods.h"
#include "../common/ThreadStart.h"
#include "../jrd/os/pio_proto.h"
#include "../common/StatusArg.h"
#include "../common/StatusHolder.h"
#include "../jrd/lck.h"
#include "../jrd/jrd.h"
#include "../jrd/pag.h"
#include "../jrd/nbak.h"
#include "../jrd/cch_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/pag_proto.h"
#include "../common/isc_proto.h"
#include "../common/classes/GetPlugins.h"

using namespace Firebird;

namespace {
	class SpareBuffer
	{
	public:
		Ods::pag* get()
		{
			return buffer;
		}

	private:
		Ods::pag buffer[MAX_PAGE_SIZE / sizeof(Ods::pag)];
	};

	THREAD_ENTRY_DECLARE cryptThreadStatic(THREAD_ENTRY_PARAM p)
	{
		Jrd::CryptoManager* cryptoManager = (Jrd::CryptoManager*)p;
		cryptoManager->cryptThread();

		return 0;
	}

	class Header
	{
	public:
		Header(Jrd::thread_db* p_tdbb, USHORT lockType)
			: tdbb(p_tdbb),
			  window(Jrd::HEADER_PAGE_NUMBER),
			  header((Ods::header_page*) CCH_FETCH(tdbb, &window, lockType, pag_header))
		{
			if (!header)
			{
				(Arg::Gds(isc_random) << "Header page fetch failed").raise();
			}
		}

		Ods::header_page* write()
		{
			CCH_MARK_MUST_WRITE(tdbb, &window);
			return header;
		}

		void depends(Stack<ULONG>& pages)
		{
			while(pages.hasData())
			{
				CCH_precedence(tdbb, &window, pages.pop());
			}
		}

		const Ods::header_page* operator->() const
		{
			return header;
		}

		operator const Ods::header_page*() const
		{
			return header;
		}

		~Header()
		{
			CCH_RELEASE(tdbb, &window);
		}

	private:
		Jrd::thread_db* tdbb;
		Jrd::WIN window;
		Ods::header_page* header;
	};

	class NoEntrypoint
	{
	public:
		virtual void FB_CARG noEntrypoint(IStatus* s)
		{
			s->set(Arg::Gds(isc_wish_list).value());
		}
	};

	MakeUpgradeInfo<NoEntrypoint> upInfo;
}

namespace Jrd {

	CryptoManager::CryptoManager(thread_db* tdbb)
		: PermanentStorage(*tdbb->getDatabase()->dbb_permanent),
		  keyHolderPlugins(getPool()),
		  cryptThreadId(0),
		  cryptPlugin(NULL),
		  dbb(*tdbb->getDatabase()),
		  needLock(true),
		  crypt(false),
		  process(false),
		  down(false)
	{
		stateLock = FB_NEW_RPT(getPool(), 0)
			Lock(tdbb, LCK_crypt_status, this, blockingAstChangeCryptState);
		threadLock = FB_NEW_RPT(getPool(), 0) Lock(tdbb, LCK_crypt);

		takeStateLock(tdbb);
	}

	CryptoManager::~CryptoManager()
	{
	}

	void CryptoManager::terminateCryptThread(thread_db*)
	{
		if (cryptThreadId)
		{
			down = true;
			Thread::waitForCompletion(cryptThreadId);
			cryptThreadId = 0;
		}
	}

	void CryptoManager::shutdown(thread_db* tdbb)
	{
		terminateCryptThread(tdbb);

		if (cryptPlugin)
		{
			PluginManagerInterfacePtr()->releasePlugin(cryptPlugin);
			cryptPlugin = NULL;
		}

		if (stateLock)
		{
			LCK_release(tdbb, stateLock);
			stateLock = NULL;
		}
	}

	void CryptoManager::takeStateLock(thread_db* tdbb)
	{
		fb_assert(stateLock);
		fb_assert(tdbb->getAttachment());

		if (needLock)
		{
			if (!LCK_lock(tdbb, stateLock, LCK_SR, LCK_WAIT))
			{
				fb_assert(tdbb->tdbb_status_vector[1]);
				ERR_punt();
			}

			fb_utils::init_status(tdbb->tdbb_status_vector);

			needLock = false;
		}
	}

	void CryptoManager::loadPlugin(const char* pluginName)
	{
		MutexLockGuard guard(pluginLoadMtx);

		if (cryptPlugin)
		{
			return;
		}

		GetPlugins<IDbCryptPlugin> cryptControl(PluginType::DbCrypt, FB_DBCRYPT_PLUGIN_VERSION,
			upInfo, dbb.dbb_config, pluginName);
		if (!cryptControl.hasData())
		{
			(Arg::Gds(isc_random) <<
			 "Invalid crypt plugin name").raise();
		}

		// do not assign cryptPlugin directly before key init complete
		IDbCryptPlugin* p = cryptControl.plugin();
		keyHolderPlugins.init(p);
		cryptPlugin = p;
		cryptPlugin->addRef();
	}

	void CryptoManager::changeCryptState(thread_db* tdbb, const Firebird::string& plugName)
	{
		if (plugName.length() > 31)
		{
			(Arg::Gds(isc_random) <<
			 "Crypt plugin name should not be >31 bytes").raise();
		}

		bool newCryptState = plugName.hasData();
		{	// window scope
			Header hdr(tdbb, LCK_write);

			// Check header page for flags
			if (hdr->hdr_flags & Ods::hdr_crypt_process)
			{
				(Arg::Gds(isc_random) <<
				 "Crypt failed - already crypting database").raise();
			}

			bool headerCryptState = hdr->hdr_flags & Ods::hdr_encrypted;
			if (headerCryptState == newCryptState)
			{
				(Arg::Gds(isc_random) <<
				 "Crypt failed - database is already in requested state").raise();
			}

			fb_assert(stateLock);
			// Take exclusive stateLock
			bool ret = needLock ? LCK_lock(tdbb, stateLock, LCK_PW, LCK_WAIT) :
								  LCK_convert(tdbb, stateLock, LCK_PW, LCK_WAIT);
			if (!ret)
			{
				fb_assert(tdbb->tdbb_status_vector[1]);
				ERR_punt();
			}
			fb_utils::init_status(tdbb->tdbb_status_vector);
			needLock = false;

			// Load plugin
			if (newCryptState)
			{
				loadPlugin(plugName.c_str());
			}
			crypt = newCryptState;

			// Write modified header page
			Ods::header_page* header = hdr.write();
			if (crypt)
			{
				header->hdr_flags |= Ods::hdr_encrypted;
				plugName.copyTo(header->hdr_crypt_plugin, sizeof header->hdr_crypt_plugin);
			}
			else
			{
				header->hdr_flags &= ~Ods::hdr_encrypted;
			}
			header->hdr_flags |= Ods::hdr_crypt_process;
			process = true;
		}

		// Trigger lock on ChangeCryptState
		if (!LCK_convert(tdbb, stateLock, LCK_EX, LCK_WAIT))
		{
			ERR_punt();
		}

		if (!LCK_convert(tdbb, stateLock, LCK_SR, LCK_WAIT))
		{
			ERR_punt();
		}
		fb_utils::init_status(tdbb->tdbb_status_vector);

		// Now we may set hdr_crypt_page for crypt thread
		{	// window scope
			Header hdr(tdbb, LCK_write);
			Ods::header_page* header = hdr.write();
			header->hdr_crypt_page = 1;
		}

		startCryptThread(tdbb);
	}

	void CryptoManager::blockingAstChangeCryptState()
	{
		AsyncContextHolder tdbb(&dbb);

		fb_assert(stateLock);
		LCK_release(tdbb, stateLock);
		needLock = true;
	}

	void CryptoManager::startCryptThread(thread_db* tdbb)
	{
		// Take exclusive threadLock
		// If can't take that lock - nothing to do, cryptThread already runs somewhere
		if (LCK_lock(tdbb, threadLock, LCK_EX, LCK_NO_WAIT))
		{
			// Cleanup resources
			terminateCryptThread(tdbb);
			down = false;

			// Determine current page from the header
			Header hdr(tdbb, LCK_read);
			process = hdr->hdr_flags & Ods::hdr_crypt_process ? true : false;
			if (!process)
			{
				LCK_release(tdbb, threadLock);
				return;
			}
			currentPage.setValue(hdr->hdr_crypt_page);

			// Refresh encryption flag
			crypt = hdr->hdr_flags & Ods::hdr_encrypted ? true : false;

			// If we are going to start crypt thread, we really need plugin to be loaded
			loadPlugin(hdr->hdr_crypt_plugin);

			// ready to go
			Thread::start(cryptThreadStatic, (THREAD_ENTRY_PARAM) this, 0, &cryptThreadId);
		}
		fb_utils::init_status(tdbb->tdbb_status_vector);
	}

	void CryptoManager::attach(thread_db* tdbb, Attachment* att)
	{
		keyHolderPlugins.attach(att, dbb.dbb_config);
	}

	void CryptoManager::cryptThread()
	{
		ISC_STATUS_ARRAY status_vector;

		try
		{
			// establish context
			UserId user;
			user.usr_user_name = "(Crypt thread)";

			Jrd::Attachment* const attachment = Jrd::Attachment::create(&dbb);
			RefPtr<SysAttachment> jAtt(new SysAttachment(attachment));
			attachment->att_interface = jAtt;
			attachment->att_filename = dbb.dbb_filename;
			attachment->att_user = &user;

			BackgroundContextHolder tdbb(&dbb, attachment, status_vector);
			tdbb->tdbb_quantum = SWEEP_QUANTUM;

			ULONG lastPage = getLastPage(tdbb);
			ULONG runpage = 1;
			Stack<ULONG> pages;

			try
			{
				do
				{
					// Check is there some job to do
					while ((runpage = currentPage.exchangeAdd(+1)) < lastPage)
					{
						// forced terminate
						if (down)
						{
							break;
						}

						// nbackup state check
						if (dbb.dbb_backup_manager && dbb.dbb_backup_manager->getState() != nbak_state_normal)
						{
							if (currentPage.exchangeAdd(-1) >= lastPage)
							{
								// currentPage was set to last page by thread, closing database
								break;
							}
							THD_sleep(100);
							continue;
						}

						// scheduling
						if (--tdbb->tdbb_quantum < 0)
						{
							JRD_reschedule(tdbb, SWEEP_QUANTUM, true);
						}

						// writing page to disk will change it's crypt status in usual way
						WIN window(DB_PAGE_SPACE, runpage);
						Ods::pag* page = CCH_FETCH(tdbb, &window, LCK_write, pag_undefined);
						if (page && page->pag_type <= pag_max &&
							(bool(page->pag_flags & Ods::crypted_page) != crypt) &&
							Ods::pag_crypt_page[page->pag_type])
						{
							CCH_MARK(tdbb, &window);
							pages.push(runpage);
						}
						CCH_RELEASE_TAIL(tdbb, &window);

						// sometimes save currentPage into DB header
						++runpage;
						if ((runpage & 0x3FF) == 0)
						{
							writeDbHeader(tdbb, runpage, pages);
						}
					}

					// At this moment of time all pages with number < lastpage
					// are guaranteed to change crypt state. Check for added pages.
					lastPage = getLastPage(tdbb);
				} while (runpage < lastPage);

				// Finalize crypt
				if (!down)
				{
					writeDbHeader(tdbb, 0, pages);
				}

				// Release exclusive lock on StartCryptThread
				LCK_release(tdbb, threadLock);
			}
			catch(const Exception&)
			{
				try
				{
					// try to save current state of crypt thread
					writeDbHeader(tdbb, runpage, pages);

					// Release exclusive lock on StartCryptThread
					LCK_release(tdbb, threadLock);
				}
				catch(const Exception&)
				{ }

				throw;
			}
		}
		catch(const Exception& ex)
		{
			// Error during context creation - we can't even release lock
			iscLogException("Crypt thread:", ex);
		}
	}

	void CryptoManager::writeDbHeader(thread_db* tdbb, ULONG runpage, Stack<ULONG>& pages)
	{
		Header hdr(tdbb, LCK_write);
		hdr.depends(pages);

		Ods::header_page* header = hdr.write();
		header->hdr_crypt_page = runpage;
		if (!runpage)
		{
			header->hdr_flags &= ~Ods::hdr_crypt_process;
			process = false;
		}
	}

	bool CryptoManager::cryptRead(jrd_file* file, BufferDesc* bdb, Ods::pag* page, ISC_STATUS* sv)
	{
		try
		{
			return bdb->bdb_bcb->bcb_database->dbb_crypto_manager->read(file, bdb, page, sv);
		}
		catch(const Exception& ex)
		{
			ex.stuff_exception(sv);
		}
		return false;
	}

	bool CryptoManager::cryptWrite(jrd_file* file, BufferDesc* bdb, Ods::pag* page, ISC_STATUS* sv)
	{
		try
		{
			return bdb->bdb_bcb->bcb_database->dbb_crypto_manager->write(file, bdb, page, sv);
		}
		catch(const Exception& ex)
		{
			ex.stuff_exception(sv);
		}
		return false;
	}

	bool CryptoManager::read(jrd_file* file, BufferDesc* bdb, Ods::pag* page, ISC_STATUS* sv)
	{
		if (!PIO_read(file, bdb, page, sv))
		{
			return false;
		}

		if (page->pag_flags & Ods::crypted_page)
		{
			if (!cryptPlugin)
			{
				// We are invoked from shared cache manager, i.e. no valid attachment in tdbb 
				// Therefore create system temporary attachment like in crypt thread to be able to work with locks
				UserId user;
				user.usr_user_name = "(Crypt plugin loader)";

				Jrd::Attachment* const attachment = Jrd::Attachment::create(&dbb);
				RefPtr<SysAttachment> jAtt(new SysAttachment(attachment));
				attachment->att_interface = jAtt;
				attachment->att_filename = dbb.dbb_filename;
				attachment->att_user = &user;

				BackgroundContextHolder tdbb(&dbb, attachment, sv);

				// Lock crypt state
				takeStateLock(tdbb);

				Header hdr(tdbb, LCK_read);
				crypt = hdr->hdr_flags & Ods::hdr_encrypted;
				process = hdr->hdr_flags & Ods::hdr_crypt_process;

				if (crypt || process)
				{
					loadPlugin(hdr->hdr_crypt_plugin);
				}

				if (!cryptPlugin)
				{
					(Arg::Gds(isc_random) << "Not crypt mode, but page appears encrypted").copyTo(sv);
					return false;
				}
			}

			LocalStatus status;
			cryptPlugin->decrypt(&status, dbb.dbb_page_size - sizeof(page[0]), &page[1], &page[1]);
			if (!status.isSuccess())
			{
				memcpy(sv, status.get(), sizeof(ISC_STATUS_ARRAY));
				return false;
			}
		}

		return true;
	}

	bool CryptoManager::write(jrd_file* file, BufferDesc* bdb, Ods::pag* page, ISC_STATUS* sv)
	{
		for (;;)
		{
			SpareBuffer spare_buffer;
			Ods::pag* buffer = page;

			if (crypt && cryptPlugin && Ods::pag_crypt_page[page->pag_type % (pag_max + 1)])
			{
				buffer = spare_buffer.get();
				buffer[0] = page[0];

				LocalStatus status;
				cryptPlugin->encrypt(&status, dbb.dbb_page_size - sizeof(page[0]), &page[1], &buffer[1]);
				if (!status.isSuccess())
				{
					memcpy(sv, status.get(), sizeof(ISC_STATUS_ARRAY));
					return false;
				}

				buffer->pag_flags |= Ods::crypted_page;
			}
			else
			{
				buffer->pag_flags &= ~Ods::crypted_page;
			}

			return PIO_write(file, bdb, buffer, sv);
		}
	}

	int CryptoManager::blockingAstChangeCryptState(void* object)
	{
		((CryptoManager*)object)->blockingAstChangeCryptState();
		return 0;
	}

	ULONG CryptoManager::getCurrentPage()
	{
		return process ? currentPage.value() : 0;
	}

	ULONG CryptoManager::getLastPage(thread_db* tdbb)
	{
		return PAG_last_page(tdbb) + 1;
	}

	CryptoManager::HolderAttachments::HolderAttachments(MemoryPool& p)
		: keyHolder(NULL), attachments(p)
	{ }

	void CryptoManager::HolderAttachments::setPlugin(IKeyHolderPlugin* kh)
	{
		keyHolder = kh;
		keyHolder->addRef();
	}

	CryptoManager::HolderAttachments::~HolderAttachments()
	{
		if (keyHolder)
		{
			PluginManagerInterfacePtr()->releasePlugin(keyHolder);
		}
	}

	void CryptoManager::HolderAttachments::registerAttachment(Attachment* att)
	{
		attachments.add(att);
	}

	IKeyHolderPlugin* CryptoManager::HolderAttachments::unregisterAttachment(Attachment* att)
	{
		for (unsigned i = 0; i < attachments.getCount(); ++i)
		{
			if (attachments[i] == att)
			{
				attachments.remove(i);
				if (attachments.getCount() == 0)
				{
					return keyHolder;		// to be removed from holdersVector
				}
				break;
			}
		}
		return NULL;
	}

	bool CryptoManager::HolderAttachments::operator==(IKeyHolderPlugin* kh) const
	{
		return MasterInterfacePtr()->same(keyHolder, kh) != 0;
	}

	void CryptoManager::KeyHolderPlugins::attach(Attachment* att, Config* config)
	{
		MutexLockGuard g(holdersMutex);

		for (GetPlugins<IKeyHolderPlugin> keyControl(PluginType::KeyHolder,
			FB_KEYHOLDER_PLUGIN_VERSION, upInfo, config); keyControl.hasData(); keyControl.next())
		{
			IKeyHolderPlugin* keyPlugin = keyControl.plugin();
			LocalStatus st;
			if (keyPlugin->keyCallback(&st, att->att_crypt_callback) == 1)
			{
				// holder accepted attachment's key
				HolderAttachments* ha = NULL;

				for (unsigned i = 0; i < knownHolders.getCount(); ++i)
				{
					if (knownHolders[i] == keyPlugin)
					{
						ha = &knownHolders[i];
						break;
					}
				}

				if (!ha)
				{
					ha = &(knownHolders.add());
					ha->setPlugin(keyPlugin);
				}

				ha->registerAttachment(att);
				break;		// Do not need >1 key from attachment to single DB
			}
			else if (!st.isSuccess())
			{
				status_exception::raise(st.get());
			}
		}
	}

	void CryptoManager::KeyHolderPlugins::detach(Attachment* att)
	{
		MutexLockGuard g(holdersMutex);

		for (unsigned i = 0; i < knownHolders.getCount(); ++i)
		{
			IKeyHolderPlugin* keyPlugin = knownHolders[i].unregisterAttachment(att);
			if (keyPlugin)
			{
				knownHolders.remove(i);
			}
		}
	}

	void CryptoManager::KeyHolderPlugins::init(IDbCryptPlugin* crypt)
	{
		MutexLockGuard g(holdersMutex);

		Firebird::HalfStaticArray<Firebird::IKeyHolderPlugin*, 64> holdersVector;
		unsigned int length = knownHolders.getCount();
		IKeyHolderPlugin** vector = holdersVector.getBuffer(length);
		for (unsigned i = 0; i < length; ++i)
		{
			vector[i] = knownHolders[i].getPlugin();
		}

		LocalStatus st;
		crypt->setKey(&st, length, vector);
		if (!st.isSuccess())
		{
			status_exception::raise(st.get());
		}
	}

} // namespace Jrd
