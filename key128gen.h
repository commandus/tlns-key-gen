#include <cstddef>
#include <cinttypes>

uint8_t* keyGen(
    uint8_t* retVal,
    uint32_t keyNumber,
    uint8_t* key,
    uint32_t devAddr
);

uint8_t* phrase2key(
    uint8_t* retVal,
    const char* phrase,
    size_t size
);
