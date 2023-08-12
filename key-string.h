#include <string>

#include "key128gen.h"

std::string hexString(
    const void *buffer,
    size_t size
);

std::string hex2string(
    const std::string &hex
);

std::string KEY2string(
	const KEY128 &value
);

void string2KEY(
	KEY128 &retval,
	const std::string &str
);