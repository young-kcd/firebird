#include "../auth/SecureRemotePassword/BigInteger.h"
#include "../common/classes/alloc.h"
#include "../common/classes/fb_string.h"
#include "../common/sha.h"

#define SRP_DEBUG 0		// >0 - prints some debug info
						// >1 - uses consts instead randoms, NEVER use in PRODUCTION!

// for HANDSHAKE_DEBUG
#include "../remote/remot_proto.h"

namespace Auth {

/*
 * Order of battle for SRP handshake:
 *
 * 													0.  At account creation, the server generates
 * 														a random salt and computes a password
 * 														verifier from the account name, password,
 * 														and salt.
*
 * 		1. Client generates random number
 * 		   as private key, computes public
 * 		   key.
 *
 * 		2. Client sends server the account
 * 		   name and its public key.
 * 													3.  Server receives account name, looks up
 * 														salt and password verifier.  Server
 * 														generates random number as private key.
 * 														Server computes public key from private
 * 														key, account name, verifier, and salt.
 *
 * 													4.  Server sends client public key and salt
 *
 * 		3. Client receives server public
 * 		   key and computes session key
 * 		   from server key, salt, account
 * 		   name, and password.
  * 												5.  Server computes session key from client
 * 														public key, client name, and verifier
 *
 * 		For full details, see http://www.ietf.org/rfc/rfc5054.txt
 *
 */

class RemoteGroup;

class Sha1 : public Firebird::Sha1
{
public:
	void getInt(BigInteger& hash)
	{
		Firebird::UCharBuffer tmp;
		getHash(tmp);
		hash.assign(tmp.getCount(), tmp.begin());
	}

	void processInt(const BigInteger& data)
	{
		Firebird::UCharBuffer bytes;
		data.getBytes(bytes);
		process(bytes);
	}

	void processStrippedInt(const BigInteger& data)
	{
		Firebird::UCharBuffer bytes;
		data.getBytes(bytes);
		unsigned int n = (bytes[0] == 0) ? 1u : 0;
		process(bytes.getCount() - n, bytes.begin() + n);
	}
};

class RemotePassword : public Firebird::GlobalStorage
{
private:
	const RemoteGroup*	group;
	Auth::Sha1			hash;
	BigInteger			privateKey;
	BigInteger			scramble;

public:
	BigInteger			clientPublicKey;
	BigInteger			serverPublicKey;

public:
	RemotePassword();

	static const char* plugName;
	static const unsigned SRP_KEY_SIZE = 128;
	static const unsigned SRP_VERIFIER_SIZE = SRP_KEY_SIZE;
	static const unsigned SRP_SALT_SIZE = 32;

	BigInteger getUserHash(const char* account,
						   const char* salt,
						   const char* password);
	BigInteger computeVerifier(const Firebird::string& account,
							   const Firebird::string& salt,
							   const Firebird::string& password);
	void genClientKey(Firebird::string& clientPubKey);
	void genServerKey(Firebird::string& serverPubKey, const Firebird::UCharBuffer& verifier);
	void computeScramble();
	void clientSessionKey(Firebird::UCharBuffer& sessionKey, const char* account,
						  const char* salt, const char* password,
						  const char* serverPubKey);
	void serverSessionKey(Firebird::UCharBuffer& sessionKey,
						  const char* clientPubKey,
						  const Firebird::UCharBuffer& verifier);
	BigInteger clientProof(const char* account,
						   const char* salt,
						   const Firebird::UCharBuffer& sessionKey);
};


#if SRP_DEBUG > 0
void dumpIt(const char* name, const BigInteger& bi);
void dumpIt(const char* name, const Firebird::UCharBuffer& data);
void dumpIt(const char* name, const Firebird::string& str);
#else
void static inline dumpIt(const char* name, const BigInteger& bi) { }
void static inline dumpIt(const char* name, const Firebird::UCharBuffer& data) { }
void static inline dumpIt(const char* name, const Firebird::string& str) { }
#endif

} // namespace Auth
