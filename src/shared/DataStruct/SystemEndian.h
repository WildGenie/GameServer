#ifndef __SYSENDIAN_H
#define __SYSENDIAN_H

/**
 * @brief ϵͳ��С�˶���
 */
enum SysEndian
{
	eSys_BIG_ENDIAN,
	eSys_LITTLE_ENDIAN
};

static union { char c[4]; unsigned long mylong; } endian_test = { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)	// (���ENDIANNESS=��l����ʾϵͳΪlittle endian,��b����ʾbig endian )

static SysEndian sSysEndian = ENDIANNESS == '1' ? eSys_LITTLE_ENDIAN : eSys_BIG_ENDIAN;	// ����ϵͳ��С��

#endif