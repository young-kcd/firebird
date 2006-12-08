#ifndef AUTH_SPPI_H
#define AUTH_SPPI_H

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

	bool isActive()
	{
		return hasContext;
	}

	bool request(DataHolder& data);
	bool accept(DataHolder& data);
	bool getLogin(Firebird::string& login);
};
#endif //TRUSTED_AUTH
#endif //AUTH_SPPI_H
