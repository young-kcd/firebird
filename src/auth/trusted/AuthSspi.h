#ifndef AUTH_SSPI_H
#define AUTH_SSPI_H

#include <firebird.h>

#ifdef TRUSTED_AUTH

#include <../common/classes/fb_string.h>
#include <../common/classes/array.h>
#include <../jrd/ibase.h>

#define SECURITY_WIN32

#include <windows.h>
#include <Security.h>
#include <stdio.h>

class AuthSspi
{
private:
	enum {BUFSIZE = 4096};

	SecHandle secHndl;
	bool hasCredentials;
	CtxtHandle ctxtHndl;
	bool hasContext;
	Firebird::string ctName;
public:
	typedef Firebird::Array<unsigned char> DataHolder;

	AuthSspi();
	~AuthSspi();

	// true when has non-empty security context, 
	// ready to be sent to the other side
	bool isActive() const
	{
		return hasContext;
	}

	// prepare security context to be sent to the server (used by client)
	bool request(DataHolder& data);
	
	// accept security context from the client (used by server)
	bool accept(DataHolder& data);

	// returns windows user name, matching accepted security context
	bool getLogin(Firebird::string& login);
};

#endif // TRUSTED_AUTH
#endif // AUTH_SSPI_H
