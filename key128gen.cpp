#include "key128gen.h"

#include <cstring>

#include "system/crypto/aes.h"
#include "system/crypto/cmac.h"

// N_MAX_ROUNDS = 14  N_BLOCK = 4 * 4 = 15
// KSCH_SIZE = 240
#define KSCH_SIZE	(N_MAX_ROUNDS + 1) * N_BLOCK

void euiGen(
    uint8_t *retVal,
    uint32_t keyNumber,
    uint8_t *key,
    uint32_t devAddr
)
{
    uint8_t k[16];
    keyGen((uint8_t *) &k, keyNumber, key, devAddr);
    memmove(retVal, &k, 8);
}

uint8_t* keyGen(
	uint8_t *retVal,
	uint32_t keyNumber,
	uint8_t *key,
	uint32_t devAddr
)
{
	uint8_t blockB[16];
	// blockB
	blockB[0] = 65;
	blockB[1] = 78;
	blockB[2] = 68;
	blockB[3] = 89;

	uint8_t* kA = (uint8_t*) &keyNumber;
	blockB[4] = kA[0];
	blockB[5] = kA[1];
	blockB[6] = kA[2];
	blockB[7] = kA[3];

	uint8_t* dA = (uint8_t*) &devAddr;
	blockB[8] = dA[0];
	blockB[9] = dA[1];
	blockB[10] = dA[2];
	blockB[11] = dA[3];

	blockB[12] = 69;
	blockB[13] = 78;
	blockB[14] = 90;
	blockB[15] = 73;

	aes_context aesContext;
	memset(aesContext.ksch, '\0', KSCH_SIZE);
	aes_set_key(key, 16, &aesContext);

	AES_CMAC_CTX aesCmacCtx;
	AES_CMAC_Init(&aesCmacCtx);
	AES_CMAC_SetKey(&aesCmacCtx, key);
	AES_CMAC_Update(&aesCmacCtx, blockB, sizeof(blockB));
	AES_CMAC_Final(retVal, &aesCmacCtx);
	return retVal;
}

uint8_t* phrase2key(
	uint8_t* retVal,
	const char *phrase,
	size_t size
)
{
	uint8_t blockB[16];
	// blockB
	blockB[0] = 65;
	blockB[1] = 78;
	blockB[2] = 68;
	blockB[3] = 89;

	blockB[4] = 0;
	blockB[5] = 0;
	blockB[6] = 0;
	blockB[7] = 0;

	blockB[8] = 0;
	blockB[9] = 0;
	blockB[10] = 0;
	blockB[11] = 0;

	blockB[12] = 69;
	blockB[13] = 78;
	blockB[14] = 90;
	blockB[15] = 73;

	aes_context aesContext;
	memset(aesContext.ksch, '\0', KSCH_SIZE);
	uint8_t key[16];
	memset(key, 0, 16);
	uint32_t sz;
	if (size < 16)
		sz = (uint32_t) size;
	else
		sz = 16;
	memmove(key, phrase, sz);

	aes_set_key(key, 16, &aesContext);
	AES_CMAC_CTX aesCmacCtx;
	AES_CMAC_Init(&aesCmacCtx);
	AES_CMAC_SetKey(&aesCmacCtx, key);
	AES_CMAC_Update(&aesCmacCtx, blockB, sizeof(blockB));
	AES_CMAC_Update(&aesCmacCtx, (uint8_t *) phrase, (uint32_t) size);
	AES_CMAC_Final(retVal, &aesCmacCtx);
	return retVal;
}
