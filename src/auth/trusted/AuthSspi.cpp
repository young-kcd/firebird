#include "AuthSspi.h"

#ifdef TRUSTED_AUTH
#include <../common/classes/ClumpletReader.h>

AuthSspi::AuthSspi()
	: hasContext(false), ctName(*getDefaultMemoryPool())
{
	TimeStamp timeOut;
	hasCredentials = AcquireCredentialsHandle(0, "NTLM", SECPKG_CRED_BOTH, 0, 0, 0, 0, 
									   &secHndl, &timeOut) == SEC_E_OK;
}

AuthSspi::~AuthSspi()
{
	if (hasContext)
	{
		DeleteSecurityContext(&ctxtHndl);
	}
	if (hasCredentials)
	{
		FreeCredentialsHandle(&secHndl);
	}
}

namespace 
{
	void makeDesc(SecBufferDesc& d, SecBuffer& b, size_t len, void* p)
	{
		b.BufferType = SECBUFFER_TOKEN;
		b.cbBuffer = len;
		b.pvBuffer = len ? p : 0;
		d.ulVersion = SECBUFFER_VERSION;
		d.cBuffers = 1;
		d.pBuffers = &b;
	}

	bool checkAdminPrivilege(PCtxtHandle phContext)
	{
		// Query access token from security context
		SecPkgContext_AccessToken spc;
		spc.AccessToken = 0;
		if (QueryContextAttributes(phContext, SECPKG_ATTR_ACCESS_TOKEN, &spc) != SEC_E_OK)
		{
			return false;
		}

		// Query required buffer size
		DWORD token_len = 0;
		GetTokenInformation(spc.AccessToken, TokenGroups, 0, 0, &token_len);

		// Query actual group information
		Firebird::Array<char> buffer;
		TOKEN_GROUPS *ptg = (TOKEN_GROUPS *)buffer.getBuffer(token_len);
		bool ok = GetTokenInformation(spc.AccessToken,
				TokenGroups, ptg, token_len, &token_len);
		FreeContextBuffer(spc.AccessToken);
		if (! ok)
		{
			return false;
		}

		// Create a System Identifier for the Admin group.
		SID_IDENTIFIER_AUTHORITY system_sid_authority = {SECURITY_NT_AUTHORITY};
		PSID admin_sid;
		if (!AllocateAndInitializeSid(&system_sid_authority, 2,
					SECURITY_BUILTIN_DOMAIN_RID,
					DOMAIN_ALIAS_RID_ADMINS,
					0, 0, 0, 0, 0, 0, &admin_sid))
		{
			return false;
		}

		// Finally we'll iterate through the list of groups for this access
		// token looking for a match against the SID we created above.
		for (DWORD i = 0; i < ptg->GroupCount; i++)
		{
			if (EqualSid(ptg->Groups[i].Sid, admin_sid))
			{
				FreeSid(admin_sid);
				return true;
			}
		}
		FreeSid(admin_sid);
		return false;
	}
}


bool AuthSspi::request(AuthSspi::DataHolder& data)
{
	if (! hasCredentials) 
	{
		data.clear();
		return false;
	}

	TimeStamp timeOut;

	char s[BUFSIZE];
	SecBuffer outputBuffer, inputBuffer;
	SecBufferDesc outputDesc, inputDesc;
	makeDesc(outputDesc, outputBuffer, sizeof(s), s);
	makeDesc(inputDesc, inputBuffer, data.getCount(), data.begin());

	ULONG fContextAttr = 0;

	SECURITY_STATUS x = InitializeSecurityContext(
		&secHndl, hasContext ? &ctxtHndl : 0, 0, 0, 0, SECURITY_NATIVE_DREP, 
		hasContext ? &inputDesc : 0, 0, &ctxtHndl, &outputDesc, &fContextAttr, &timeOut);
	switch (x)
	{
	case SEC_E_OK:
		DeleteSecurityContext(&ctxtHndl);
		hasContext = false;
		break;
	case SEC_I_CONTINUE_NEEDED:
		hasContext = true;
		break;
	default:
		if (hasContext)
		{
			DeleteSecurityContext(&ctxtHndl);
		}
		hasContext = false;
		data.clear();
		return false;
	}

	if (outputBuffer.cbBuffer)
	{
		memcpy(data.getBuffer(outputBuffer.cbBuffer), 
			   outputBuffer.pvBuffer, outputBuffer.cbBuffer);
	}
	else
	{
		data.clear();
	}

	return true;
}

bool AuthSspi::accept(AuthSspi::DataHolder& data)
{
	if (! hasCredentials) 
	{
		data.clear();
		return false;
	}

	TimeStamp timeOut;

	char s[BUFSIZE];
	SecBuffer outputBuffer, inputBuffer;
	SecBufferDesc outputDesc, inputDesc;
	makeDesc(outputDesc, outputBuffer, sizeof(s), s);
	makeDesc(inputDesc, inputBuffer, data.getCount(), data.begin());

	ULONG fContextAttr = 0;
	SecPkgContext_Names name;
	SECURITY_STATUS x = AcceptSecurityContext(
		&secHndl, hasContext ? &ctxtHndl : 0, &inputDesc, 0, 
		SECURITY_NATIVE_DREP, &ctxtHndl, &outputDesc, 
		&fContextAttr, &timeOut);
	switch(x) 
	{
	case SEC_E_OK:
		if (checkAdminPrivilege(&ctxtHndl))
		{
			ctName = "SYSDBA";
		}
		else if (QueryContextAttributes(&ctxtHndl, SECPKG_ATTR_NAMES, &name) == SEC_E_OK)
		{
			ctName = name.sUserName;
			ctName.upper();
			FreeContextBuffer(name.sUserName);
		}
		DeleteSecurityContext(&ctxtHndl);
		hasContext = false;
		break;
	case SEC_I_CONTINUE_NEEDED:
		hasContext = true;
		break;
	default:
		if (hasContext)
		{
			DeleteSecurityContext(&ctxtHndl);
		}
		hasContext = false;
		data.clear();
		return false;
	}

	if (outputBuffer.cbBuffer)
	{
		memcpy(data.getBuffer(outputBuffer.cbBuffer), 
			   outputBuffer.pvBuffer, outputBuffer.cbBuffer);
	}
	else
	{
		data.clear();
	}

	return true;
}

bool AuthSspi::getLogin(Firebird::string& login)
{
	if (ctName.hasData())
	{
		login = ctName;
		ctName.erase();
		return true;
	}
	return false;
}

#endif // TRUSTED_AUTH
