#include "lorawan-types.h"

#ifdef ESP_PLATFORM
    #include <arpa/inet.h>
#endif
#ifdef _MSC_VER
    #include <WinSock2.h>
#endif

#ifdef _MSC_VER
    #define SWAP_BYTES_2(x) htons(x)
    #define SWAP_BYTES_4(x) htonl(x)
    #define SWAP_BYTES_8(x) htonll(x)
#else
    #ifdef ESP_PLATFORM
        #define SWAP_BYTES_2(x) htons(x)
        #define SWAP_BYTES_4(x) htonl(x)
        #define SWAP_BYTES_8(x) ((((uint64_t) htonl(x)) << 32) + htonl((uint64_t)(x) >> 32))
    #else
        #define SWAP_BYTES_2(x) be16toh(x)
        #define SWAP_BYTES_4(x) be32toh(x)
        #define SWAP_BYTES_8(x) be64toh(x)
    #endif
#endif

#if BYTE_ORDER == BIG_ENDIAN
    #define NTOH2(x) (x)
    #define NTOH4(x) (x)
    #define NTOH8(x) (x)
    #define HTON2(x) (x)
    #define HTON4(x) (x)
    #define HTON8(x) (x)
#else
    #ifdef ESP_PLATFORM
        #define NTOH2(x) htons(x)
        #define NTOH4(x) htonl(x)
        #define NTOH8(x) ((((uint64_t)htonl(x)) << 32) + htonl((uint64_t)(x) >> 32))
        #define HTON2(x) htons(x)
        #define HTON4(x) htonl(x)
        #define HTON8(x) ((((uint64_t)htonl(x)) << 32) + htonl((uint64_t)(x) >> 32))
    #else
        #define NTOH2(x) be16toh(x)
        #define NTOH4(x) be32toh(x)
        #define NTOH8(x) be64toh(x)
        #define HTON2(x) htobe16(x)
        #define HTON4(x) htobe32(x)
        #define HTON8(x) htobe64(x)
    #endif
#endif

bool isDEVADDREmpty(const DEVADDR &addr);
bool isDEVEUIEmpty(const DEVEUI &eui);

uint32_t DEVADDR2int(const DEVADDR &value);
void int2DEVADDR(DEVADDR &retval, uint32_t value);

uint32_t NETID2int(const NETID &value);
void int2NETID(NETID &retval, uint32_t value);

uint32_t JOINNONCE2int(const JOINNONCE &value);

int FREQUENCY2int(const FREQUENCY &frequency);

void int2JOINNONCE(JOINNONCE &retVal, int value);

void int2APPNONCE(APPNONCE& retVal, int value);
