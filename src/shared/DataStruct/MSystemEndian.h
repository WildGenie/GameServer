#ifndef __SYSENDIAN_H
#define __SYSENDIAN_H

/**
 * @brief 系统大小端定义
 */
enum SysEndian
{
	eSys_BIG_ENDIAN,
	eSys_LITTLE_ENDIAN
};

static union { char c[4]; unsigned long mylong; } endian_test = { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)	// (如果ENDIANNESS == 'l',是字母'l'，不是数字'1'，表示系统为little endian,'b'表示big endian )

static SysEndian sSysEndian = ENDIANNESS == 'l' ? eSys_LITTLE_ENDIAN : eSys_BIG_ENDIAN;	// 决定系统大小端

#endif