#include "key128gen.h"
#include "key-string.h"

#include "system/crypto/aes.h"
#include "system/crypto/cmac.h"


KEY128::KEY128()
{
    memset(&c, 0, 16);
}
KEY128::KEY128(
    const std::string &hex
) {
    string2KEY(*this, hex);
}

KEY128::KEY128(
    const char* hex
)
{
    string2KEY(*this, hex);
}

KEY128::KEY128(
    uint64_t hi,
    uint64_t lo
)
{
#if BYTE_ORDER == BIG_ENDIAN
    u[0] = lo;
    u[1] = hi;
#else
    u[1] = NTOH8(lo);
    u[0] = NTOH8(hi);
#endif

}

KEY128::KEY128(
    KEY128 &value
)
{
    memmove(&c, &value, 16);
}

std::size_t KEY128::operator()(const KEY128 &value) const {
    return std::hash<std::size_t>{}(u[0] ^  u[1]);
}

bool KEY128::operator==(const KEY128 &rhs) const {
    return memcmp(&c, &rhs.c, 16) == 0;
}
bool KEY128::operator<(const KEY128 &rhs) const {
    return memcmp(&rhs.c, &c, 16) < 0;
}
bool KEY128::operator>(const KEY128 &rhs) const {
    return memcmp(&rhs.c, &c, 16) > 0;
}
bool KEY128::operator!=(const KEY128 &rhs) const {
    return memcmp(&rhs.c, &c, 16) != 0;
}

uint32_t calc()
{
	/*
		const DEVEUI &joinEUI,
	const JOINNONCE &devNonce,
	const KEY128 &key,
	const LORAWAN_JOIN_ACCEPT *data,
	bool hasCFList

	uint32_t r;
	JOINACCEPT4MIC_NEG1 a;
	size_t sz = 0;
	if (data->s.header.optneg) {
		// JoinReqType | JoinEUI | DevNonce 12 bytes
		a.n.joinType = (uint8_t) JOINREQUEST;
		memmove(a.n.eui, joinEUI, sizeof(DEVEUI));
		memmove(a.n.devnonce, devNonce, sizeof(JOINNONCE));
		sz += sizeof(JOINACCEPT4MIC_NEG1_HEADER);
	}
	//  MHDR | JoinNonce | NetID | DevAddr | DLSettings | RxDelay: 13 bytes | CFList: 16 bytes
	a.v.macheader = data->s.header.macheader;
	sz += sizeof(LORAWAN_JOIN_ACCEPT_HEADER);
	if (hasCFList)
		sz += sizeof(CFLIST);
	std::cerr << "size of " << sz << std::endl;		
	memmove(a.v.joinnonce, data->s.header.joinNonce, sz);

	unsigned char blockB[16];
	memset(blockB, 0, sizeof(blockB));
	aes_context aesContext;
	memset(aesContext.ksch, '\0', 240);
	AES_CMAC_CTX aesCmacCtx;
    aes_set_key(key, sizeof(KEY128), &aesContext);
	AES_CMAC_Init(&aesCmacCtx);
	AES_CMAC_SetKey(&aesCmacCtx, key);
	AES_CMAC_Update(&aesCmacCtx, blockB, sizeof(blockB));
	void *p;
	if (data->s.header.optneg) {
		p = &a.n;
	} else {
		p = &a.v;
	}
	std::cerr << "size of " << sz << std::endl;

	AES_CMAC_Update(&aesCmacCtx, (const uint8_t *) p, (uint32_t) sz);
	uint8_t mic[16];
	AES_CMAC_Final(mic, &aesCmacCtx);
    r = (uint32_t) ((uint32_t)mic[3] << 24 | (uint32_t)mic[2] << 16 | (uint32_t)mic[1] << 8 | (uint32_t)mic[0] );
	return r;
		*/
	return 0;
}
