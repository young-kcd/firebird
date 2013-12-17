#include "../common/classes/alloc.h"
#include "../common/classes/auto.h"
#include "../common/StatusHolder.h"
#include "../common/MsgMetadata.h"


// This class helps to work with metadata iface
class Meta : public Firebird::RefPtr<Firebird::IMessageMetadata>
{
public:
	explicit Meta(Firebird::IStatement* stmt, bool out)
	{
		Firebird::LocalStatus s;
		Firebird::IMessageMetadata* m = out ? stmt->getOutputMetadata(&s) : stmt->getInputMetadata(&s);
		if (!s.isSuccess())
		{
			Firebird::status_exception::raise(s.get());
		}
		assignRefNoIncr(m);
	}
};


// This class helps to fill message with correct values
class Message : public Firebird::GlobalStorage
{
public:
	Message(Firebird::IMessageMetadata* aMeta)
		: dataBuf(getPool()), fieldCount(0)
	{
		Firebird::LocalStatus st;
		buffer = dataBuf.getBuffer(aMeta->getMessageLength(&st));
		check(&st);
		metadata = aMeta;
	}

	~Message()
	{ }

	template <typename T>
	static bool checkType(unsigned t, unsigned /*sz*/)
	{
		return T::unknownDataType();
	}

	template <typename T>
	unsigned add(unsigned& t, unsigned& sz)
	{
		Firebird::LocalStatus st;

		unsigned l = metadata->getCount(&st);
		check(&st);
		if (fieldCount >= metadata->getMessageLength(&st))
		{
			(Firebird::Arg::Gds(isc_random) <<
				"Attempt to add to the message more variables than possible").raise();
		}

		t = metadata->getType(&st, fieldCount);
		check(&st);
		sz = metadata->getLength(&st, fieldCount);
		check(&st);
		if (!checkType<T>(t, sz))
		{
			(Firebird::Arg::Gds(isc_random) << "Incompatible data type").raise();
		}

		return fieldCount++;
	}

	static void check(Firebird::IStatus* status)
	{
		if (!status->isSuccess())
		{
			Firebird::status_exception::raise(status->get());
		}
	}

	// makes it possible to use metadata in ?: operator
	Firebird::IMessageMetadata* getMetadata()
	{
		return metadata;
	}

public:
	Firebird::RefPtr<Firebird::IMessageMetadata> metadata;
	UCHAR* buffer;

private:
	Firebird::UCharBuffer dataBuf;
	unsigned fieldCount;
};


template <>
bool Message::checkType<SLONG>(unsigned t, unsigned sz)
{
	return t == SQL_LONG && sz == sizeof(SLONG);
}

template <>
bool Message::checkType<ISC_QUAD>(unsigned t, unsigned sz)
{
	return (t == SQL_BLOB || t == SQL_QUAD) && sz == sizeof(ISC_QUAD);
}

template <>
bool Message::checkType<bool>(unsigned t, unsigned sz)
{
	return t == SQL_BOOLEAN && sz == sizeof(bool);
}


// With template magic, we make the fields strongly-typed.
template <typename T>
class Field
{
public:
	class Null
	{
	public:
		Null()
			: ptr(NULL)
		{ }

		void linkMessage(short* p)
		{
			ptr = p;
			*ptr = -1;	// mark as null initially
		}

		operator FB_BOOLEAN() const
		{
			return (*ptr) ? FB_TRUE : FB_FALSE;
		}

		FB_BOOLEAN operator=(FB_BOOLEAN val)
		{
			*ptr = val ? -1 : 0;
			return val;
		}

	private:
		short* ptr;
	};

	explicit Field(Message& m)
		: ptr(NULL), type(0), size(0)
	{
		unsigned ind = m.add<T>(type, size);

		Firebird::LocalStatus st;
		unsigned tmp = m.metadata->getOffset(&st, ind);
		Message::check(&st);
		ptr = (T*) (m.buffer + tmp);

		tmp = m.metadata->getNullOffset(&st, ind);
		Message::check(&st);
		null.linkMessage((short*) (m.buffer + tmp));
	}

	operator T()
	{
		return *ptr;
	}

	T* operator&()
	{
		return ptr;
	}

	T operator= (T newVal)
	{
		*ptr = newVal;
		null = FB_FALSE;
		return newVal;
	}

	operator const char*()
	{
		if (!charBuffer)
		{
			charBuffer.reset(FB_NEW(*getDefaultMemoryPool()) char[size + 1]);
		}

		getStrValue(charBuffer);
		return charBuffer;
	}

	const char* operator= (const char* newVal)
	{
		setStrValue(newVal, strlen(newVal));
		null = FB_FALSE;
		return newVal;
	}

	void set(unsigned length, const void* newVal)
	{
		setStrValue(newVal, length);
		null = FB_FALSE;
	}

private:
	void getStrValue(char* to)
	{
		T::incompatibleDataType();
		//(Firebird::Arg::Gds(isc_random) << "Incompatible data type").raise();
	}

	void setStrValue(const void* from, unsigned len)
	{
		T::incompatibleDataType();
		//(Firebird::Arg::Gds(isc_random) << "Incompatible data type").raise();
	}

	T* ptr;
	Firebird::AutoPtr<char, Firebird::ArrayDelete<char> > charBuffer;

public:
	Null null;
	unsigned type, size;
};

struct Varying
{
	short len;
	char data[1];
};

template <>
bool Message::checkType<Varying>(unsigned t, unsigned /*sz*/)
{
	return t == SQL_VARYING;
}

template<>
void Field<Varying>::getStrValue(char* to)
{
	unsigned len = ptr->len;
	if (len > size)
		len = size;
	memcpy(to, ptr->data, len);
	to[len] = 0;
}

template<>
void Field<Varying>::setStrValue(const void* from, unsigned len)
{
	if (len > size)
		len = size;
	memcpy(ptr->data, from, len);
	ptr->len = len;
}

struct Text
{
	char data[1];
};

template <>
bool Message::checkType<Text>(unsigned t, unsigned /*sz*/)
{
	return t == SQL_TEXT;
}

template<>
void Field<Text>::getStrValue(char* to)
{
	memcpy(to, ptr->data, size);
	to[size] = 0;
	unsigned len = size;
	while (len--)
	{
		if (to[len] == ' ')
			to[len] = 0;
		else
			break;
	}
}

template<>
void Field<Text>::setStrValue(const void* from, unsigned len)
{
	if (len > size)
		len = size;
	memcpy(ptr->data, from, len);
	if (len < size)
		memset(&ptr->data[len], ' ', size - len);
}
