/*
 *	PROGRAM:	Object oriented API samples.
 *	MODULE:		07.blob.cpp
 *	DESCRIPTION:	A sample of loading data into blob and reading.
 *					Run second time (when database already exists) to see
 *					how FbException is caught and handled by this code.
 *
 *					Example for the following interfaces:
 *					IAttachment - use of open and create blob methods
 *					IBlob - interface to work with blobs
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Initial Developer of the Original Code is Adriano dos Santos Fernandes.
 * Portions created by the Initial Developer are Copyright (C) 2011 the Initial Developer.
 * All Rights Reserved.
 *
 * Contributor(s):
 *	Alexander Peshkov
 */

#include "ifaceExamples.h"

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

static IMaster* master = fb_get_master_interface();

namespace
{
	class Event : public IEventCallbackImpl<Event, ThrowStatusWrapper>
	{
	public:
		Event(IAttachment* aAttachment, const char* name)
			: refCounter(0),
			  attachment(aAttachment),
			  counter(0),
			  status(master->getStatus()),
			  eventBlock(NULL),
			  events(NULL)
		{
			const char* names[] = {name, NULL};
			eventBlock = master->getUtilInterface()->createEventBlock(&status, names);
			events = attachment->queEvents(&status, this, eventBlock->getLength(), eventBlock->getValues());
		}

		void process(int pass)
		{
			if (!events)
				return;

			if (counter)
			{
				eventBlock->counts();
				unsigned tot = eventBlock->getCounters()[0];
				printf("Event count on pass %d is %d\n", pass, tot);

				events->release();
				events = NULL;
				counter = 0;
				events = attachment->queEvents(&status, this, eventBlock->getLength(), eventBlock->getValues());
			}
			else
				printf("Pass %d - no events\n", pass);
		}

		// refCounted implementation
		virtual void addRef()
		{
			++refCounter;
		}

		virtual int release()
		{
			if (--refCounter == 0)
			{
				delete this;
				return 0;
			}
			else
				return 1;
		}

		// IEventCallback implementation
		void eventCallbackFunction(unsigned int length, const ISC_UCHAR* data)
		{
			memcpy(eventBlock->getBuffer(), data, length);
			++counter;
			printf("AST called\n");
		}

	private:
		~Event()
		{
			if (events)
				events->release();
			eventBlock->dispose();
			status.dispose();
		}

		FbSampleAtomic refCounter;
		IAttachment* attachment;
		volatile int counter;
		ThrowStatusWrapper status;
		IEventBlock* eventBlock;
		IEvents* events;
	};
}

int main()
{
	int rc = 0;

	// set default password if none specified in environment
	setenv("ISC_USER", "sysdba", 0);
	setenv("ISC_PASSWORD", "masterkey", 0);

	// With ThrowStatusWrapper passed as status interface FbException will be thrown on error
	ThrowStatusWrapper status(master->getStatus());

	// Declare pointers to required interfaces
	IProvider* prov = master->getDispatcher();
	IAttachment* att = NULL;
	ITransaction* tra = NULL;
	Event* event = NULL;

	try
	{
		// create database
		att = prov->attachDatabase(&status, "employee", 0, NULL);

		// register an event
		event = new Event(att, "EVENT1");
		event->addRef();

		const char cmdBlock[] = "execute block as begin post_event 'EVENT1'; end";

		for (int i = 0; i < 3; ++i)
		{
#ifndef WIN32
			sleep(1);		// sec
#else
			Sleep(1000);	// msec
#endif
			event->process(i);

			tra = att->startTransaction(&status, 0, NULL);
			att->execute(&status, tra, 0, cmdBlock, SAMPLES_DIALECT,
				NULL, NULL, NULL, NULL);
			tra->commit(&status);
			tra = NULL;
		}

		// cleanup
		event->release();
		event = NULL;
		att->detach(&status);
		att = NULL;
	}
	catch (const FbException& error)
	{
		// handle error
		rc = 1;

		char buf[256];
		master->getUtilInterface()->formatStatus(buf, sizeof(buf), error.getStatus());
		fprintf(stderr, "%s\n", buf);
	}

	// release interfaces after error caught
	if (event)
		event->release();
	if (tra)
		tra->release();
	if (att)
		att->release();

	// generic cleanup
	prov->release();
	status.dispose();
}
