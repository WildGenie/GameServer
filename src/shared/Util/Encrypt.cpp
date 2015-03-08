#include "Encrypt.h"
#include <string.h>

DES_cblock EncryptDecryptUtil::sIVec = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void EncryptDecryptUtil::encrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey, NSEncryptDecrypt::EncryptDecryptMethod method, NSEncryptDecrypt::EncryptDecryptMode mode)
{
	if (method == NSEncryptDecrypt::DES)
	{
		if (mode == NSEncryptDecrypt::DES_ECB)
		{
			Des_Ecb_Encrypt(srcBytes, srcLen, destBytes, encryptKey);
		}
		else if (mode == NSEncryptDecrypt::DES_CBC)
		{
			Des_Cbc_Encrypt(srcBytes, srcLen, destBytes, encryptKey);
		}
	}
}

void EncryptDecryptUtil::decrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey, NSEncryptDecrypt::EncryptDecryptMethod method, NSEncryptDecrypt::EncryptDecryptMode mode)
{
	if (method == NSEncryptDecrypt::DES)
	{
		if (mode == NSEncryptDecrypt::DES_ECB)
		{
			Des_Ecb_Decrypt(srcBytes, srcLen, destBytes, encryptKey);
		}
		else if (mode == NSEncryptDecrypt::DES_CBC)
		{
			Des_Cbc_Decrypt(srcBytes, srcLen, destBytes, encryptKey);
		}
	}
}

void EncryptDecryptUtil::Des_Ecb_Encrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey)
{
	DES_key_schedule schedule;
	DES_set_key_unchecked((const_DES_cblock*)encryptKey, &schedule);

	unsigned char padChar = '\0';
	uint32 alignLen = (srcLen / 8 + (srcLen % 8 ? 1 : 0)) * 8;
	memset(srcBytes + srcLen, padChar, 8 - srcLen % 8);

	uint32 count = srcLen / 8;
	uint32 idx = 0;
	for (idx = 0; idx < count; idx++)
	{
		DES_ecb_encrypt((const_DES_cblock*)(srcBytes + 8 * idx), (DES_cblock*)(destBytes + 8 * idx), &schedule, DES_ENCRYPT);
	}

	memcpy(srcBytes + 8 * idx, destBytes + 8 * idx, 8 - srcLen % 8);
}

void EncryptDecryptUtil::Des_Ecb_Decrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey)
{
	DES_key_schedule schedule;
	DES_set_key_unchecked((const_DES_cblock*)encryptKey, &schedule);

	unsigned char padChar = '\0';
	uint32 alignLen = (srcLen / 8 + (srcLen % 8 ? 1 : 0)) * 8;
	memset(srcBytes + srcLen, padChar, 8 - srcLen % 8);

	uint32 count = srcLen / 8;
	uint32 idx = 0;
	for (idx = 0; idx < count; idx++)
	{
		DES_ecb_encrypt((const_DES_cblock*)(srcBytes + 8 * idx), (DES_cblock*)(destBytes + 8 * idx), &schedule, DES_DECRYPT);
	}

	memcpy(srcBytes + 8 * idx, destBytes + 8 * idx, 8 - srcLen % 8);
}

void EncryptDecryptUtil::Des_Cbc_Encrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey)
{
	DES_key_schedule schedule;
	DES_set_key_unchecked((const_DES_cblock*)encryptKey, &schedule);

	unsigned char padChar = '\0';
	uint32 alignLen = (srcLen / 8 + (srcLen % 8 ? 1 : 0)) * 8;
	memset(srcBytes + srcLen, padChar, 8 - srcLen % 8);

	uint32 count = srcLen / 8;
	uint32 idx = 0;
	for (idx = 0; idx < count; idx++)
	{
		DES_ncbc_encrypt((const unsigned char*)(srcBytes + 8 * idx), (unsigned char*)(destBytes + 8 * idx), 8, &schedule, &sIVec, DES_ENCRYPT);
	}

	memcpy(srcBytes + 8 * idx, destBytes + 8 * idx, 8 - srcLen % 8);
}

void EncryptDecryptUtil::Des_Cbc_Decrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey)
{
	DES_key_schedule schedule;
	DES_set_key_unchecked((const_DES_cblock*)encryptKey, &schedule);

	unsigned char padChar = '\0';
	uint32 alignLen = (srcLen / 8 + (srcLen % 8 ? 1 : 0)) * 8;
	memset(srcBytes + srcLen, padChar, 8 - srcLen % 8);

	uint32 count = srcLen / 8;
	uint32 idx = 0;
	for (idx = 0; idx < count; idx++)
	{
		DES_ncbc_encrypt((const unsigned char*)(srcBytes + 8 * idx), (unsigned char*)(destBytes + 8 * idx), 8, &schedule, &sIVec, DES_DECRYPT);
	}

	memcpy(srcBytes + 8 * idx, destBytes + 8 * idx, 8 - srcLen % 8);
}