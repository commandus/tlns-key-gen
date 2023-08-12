#include <iostream>
#include <iomanip>
#include <sstream>

#include "key-string.h"

// http://stackoverflow.com/questions/673240/how-do-i-print-an-unsigned-char-as-hex-in-c-using-ostream
struct HexCharStruct
{
	unsigned char c;
	explicit HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
	return (o << std::setfill('0') << std::setw(2) << std::hex << (int) hs.c);
}

inline HexCharStruct hex(unsigned char c)
{
	return HexCharStruct(c);
}

static void bufferPrintHex(std::ostream &ostream, const void* value, size_t size)
{
	if (!value)
		return;
	auto *p = (unsigned char*) value;
	for (size_t i = 0; i < size; i++)
	{
		ostream << hex(*p);
		p++;
	}
}

std::string hexString(const void *buffer, size_t size)
{
	std::stringstream r;
	bufferPrintHex(r, buffer, size);
	return r.str();
}

static std::string readHex(std::istream &s)
{
	std::stringstream r;
	s >> std::noskipws;
	char c[3] = {0, 0, 0};
	while (s >> c[0])
	{
		if (!(s >> c[1]))
			break;
		auto x = (unsigned char) strtol(c, nullptr, 16);
		r << x;
	}
	return r.str();
}

std::string hex2string(const std::string &hex)
{
	std::stringstream ss(hex);
    return readHex(ss);
}

std::string KEY2string(
	const KEY128 &value
)
{
	return hexString(&value, sizeof(value));
}

void string2KEY(
	KEY128 &retval,
	const std::string &str
)
{
	size_t len = str.size();
	std::string v;
	if (len > sizeof(KEY128))
		v = hex2string(str);
	else
		v= str;
	len = v.size();
	if (len > sizeof(KEY128))
		len = sizeof(KEY128);
	memmove(&retval.c, v.c_str(), len);
	if (len < sizeof(KEY128))
		memset(&retval.c + len, 0, sizeof(KEY128) - len);
}
