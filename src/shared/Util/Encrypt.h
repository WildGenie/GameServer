#ifndef __ENCRYPT_H
#define __ENCRYPT_H

#include "Platform/Define.h"
#include "openssl/des.h"		// openssl 解密解密

// 加密解密类型
namespace NSEncryptDecrypt
{
	enum EncryptDecryptMode
	{
		DES_ECB,			// ecb 模式
		DES_CBC,			// cbc 模式
	};

	enum EncryptDecryptMethod
	{
		DES,			// des 方法
	};
}


class EncryptDecryptUtil
{
public:
	static DES_cblock sIVec;

	static void encrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey, NSEncryptDecrypt::EncryptDecryptMethod method = NSEncryptDecrypt::DES, NSEncryptDecrypt::EncryptDecryptMode mode = NSEncryptDecrypt::DES_ECB);
	static void decrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey, NSEncryptDecrypt::EncryptDecryptMethod method = NSEncryptDecrypt::DES, NSEncryptDecrypt::EncryptDecryptMode mode = NSEncryptDecrypt::DES_ECB);

	static void Des_Ecb_Encrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey);
	static void Des_Ecb_Decrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey);

	static void Des_Cbc_Encrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey);
	static void Des_Cbc_Decrypt(char* srcBytes, uint32 srcLen, char* destBytes, const char* encryptKey);
};
 

#endif