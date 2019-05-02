#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>
/*typedef unsigned int u32;
typedef   		 int s32;
typedef unsigned short u16;
typedef 		 short s16;
typedef unsigned char u8;
typedef 		 char s8;
*/

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef uint pte_t;

#define low_16(address) (uint16_t)((address)& 0xFFFF)
#define high_16(address) (uint16_t)(((address)>>16)& 0xFFFF)

#endif


