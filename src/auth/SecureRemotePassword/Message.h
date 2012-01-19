#include "../jrd/align.h"
#include "../common/classes/alloc.h"

// This class helps to fill FbMessage with correct values
class Message : public Firebird::FbMessage, public Firebird::GlobalStorage
{
public:
	Message()
		: blrBuf(getPool()), dataBuf(getPool())
	{
		blrLength = 0;
		blr = NULL;
		bufferLength = 0;
		buffer = NULL;

		// start message BLR
		blrBuf.add(blr_version5);
		blrBuf.add(blr_begin);
		blrBuf.add(blr_message);
		blrBuf.add(0);
		countOffset = blrBuf.getCount();
		blrBuf.add(0);
		blrBuf.add(0);

		varCount = 0;
	}

	template <typename T>
	unsigned genBlr()
	{
		// for special types call type-specific BLR generator
		// for generic types use specialization of whole call
		return T::genBlr(blrBuf);
	}

	template <typename T>
	void add(unsigned& pos, unsigned& null)
	{
		if (blr)
		{
			(Firebird::Arg::Gds(isc_random) << "This is already constructed message").raise();
		}

		// generate code for variable
		unsigned align = genBlr<T>();
		if (align)
		{
			bufferLength = FB_ALIGN(bufferLength, align);
		}
		pos = bufferLength;
		bufferLength += sizeof(T);

		// generate code for null flag
		blrBuf.add(blr_short);
		blrBuf.add(0);
		align = type_alignments[dtype_short];
		if (align)
		{
			bufferLength = FB_ALIGN(bufferLength, align);
		}
		null = bufferLength;
		bufferLength += sizeof(short);

		++varCount;
	}

	void ready()
	{
		if (blr)
			return;

		// Adjust number of variables
		blrBuf[countOffset] = (varCount * 2) & 0xFF;
		blrBuf[countOffset + 1] = ((varCount * 2) >> 8) & 0xFF;

		// Complete blr
		blrBuf.add(blr_end);
		blrBuf.add(blr_eoc);
		blrLength = blrBuf.getCount();
		blr = blrBuf.begin();

		// Allocate buffer
		buffer = dataBuf.getBuffer(bufferLength);
	}

	Firebird::UCharBuffer blrBuf, dataBuf;
	unsigned countOffset, varCount;
};

template <>
unsigned Message::genBlr<SLONG>()
{
	blrBuf.add(blr_long);
	blrBuf.add(0);		// scale
	return type_alignments[dtype_long];
}

// With template magic, we make the fields strongly-typed.
template <class T>
class Field
{
public:
	explicit Field(Message& m)
		: msg(m), pos(~0), nullPos(~0)
	{
		msg.add<T>(pos, nullPos);
	}

	T& operator()()
	{
		msg.ready();
		return *(T*) (msg.buffer + pos);
	}

	short& null()
	{
		msg.ready();
		return *(short*) (msg.buffer + nullPos);
	}

private:
	Message& msg;
	unsigned pos, nullPos;
};

template <short N>
class VarChar
{
public:
	short len;
	char data[N];		// This guarantees N > 0

	static unsigned genBlr(Firebird::UCharBuffer& blr)
	{
		blr.add(blr_varying);
		blr.add(N & 0xFF);
		blr.add((N >> 8) & 0xFF);
		return type_alignments[dtype_varying];
	}

	const VarChar& operator=(const char* str)
	{
		strncpy(data, str, N);
		len = strlen(str);
		if (len > N)
			len = N;
		return *this;
	}

	void set(unsigned short l, void* bytes)
	{
		if (l > (unsigned short) N)
			l = N;
		memcpy(data, bytes, l);
		len = l;
	}
};
