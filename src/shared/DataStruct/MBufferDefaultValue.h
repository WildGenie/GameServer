#ifndef __MBUFFERDEFAULTVALUE_H
#define __MBUFFERDEFAULTVALUE_H

#define MAX_CAPACITY 8 * 1024 * 1024    // 最大允许分配 8 M
#define INIT_CAPACITY 1 * 1024          // 默认分配 1 K
#define MSG_HEADER_SIZE 4				// 包长度占据几个字节

#define PACKET_ZIP_MIN = 32;			// 压缩的最小包
#define PACKET_ZIP = 0x40000000;		// 压缩标志位

#endif		// __MBUFFERDEFAULTVALUE_H