/*
----------------------------------------------------------------------
comint.h
	define common int type 
----------------------------------------------------------------------
problem
	for uint, /usr/include/sys/types.h:153 has defined.
	how to make 'uint' not be ambiguous if not using MCS::uint
history
	2009-06-19
*/
#ifndef COM_INT_TYPE_H
#define COM_INT_TYPE_H

#include <sys/types.h>
/*
typedef char 				int8_t;
typedef short int 			int16_t;
typedef int 				int32_t;
typedef long int 			int64_t;

typedef unsigned char 		u_int8_t;
typedef unsigned short int 	u_int16_t;
typedef unsigned int 		u_int32_t;
typedef unsigned long int 	u_int64_t;

typedef unsigned short int 	ushort;
typedef unsigned int 		uint;
typedef unsigned long int 	ulong;
*/

#if (defined Linux || defined linux || defined LINUX || defined __LINUX__)
	#include <stdint.h>
#elif (defined SunOS || defined sunOS)
    #include <sys/int_types.h>
#elif (defined HP-UX)
	#include <inttypes.h>
#else
	#error __FILE__ ":unsupported platform";
#endif


#ifndef I4
	typedef uint64_t		I8;
	typedef unsigned int	I4;
	typedef unsigned short	I2;
	typedef unsigned char   I1;
#endif

#ifndef i32
	typedef int64_t			i64;
	typedef int				i32;
	typedef short			i16;
	typedef char   			i8;
#endif

#ifndef bit32
	typedef int64_t			bit64;
	typedef int				bit32;
	typedef short			bit16;
	typedef char   			bit8;
#endif

#ifndef U4
	typedef uint64_t		U8;
	typedef unsigned int	U4;
	typedef unsigned short	U2;
	typedef unsigned char   U1;
#endif

#ifndef u32
	typedef uint64_t		u64;
	typedef unsigned int	u32;
	typedef unsigned short	u16;
	typedef unsigned char   u8;
#endif

#ifndef ubit32
	typedef uint64_t		ubit64;
	typedef unsigned int	ubit32;
	typedef unsigned short	ubit16;
	typedef unsigned char   ubit8;
#endif

#ifndef INT32
	typedef int64_t			INT64;
	typedef int				INT32;
	typedef short			INT16;
	typedef char   			INT8;
#endif

#ifndef int32
	typedef int64_t			int64;
	typedef int				int32;
	typedef short			int16;
	typedef char   			int8;
#endif

#ifndef UINT32
	typedef uint64_t		UINT64;
	typedef unsigned int	UINT32;
	typedef unsigned short	UINT16;
	typedef unsigned char 	UINT8;
#endif

#ifndef uint32
	typedef uint64_t		uint64;
	typedef unsigned int	uint32;
	typedef unsigned short	uint16;
	typedef unsigned char 	uint8;
#endif

#ifndef uint
	typedef uint64_t			ullong;
	typedef unsigned int   		uint;
	typedef unsigned short  	ushort;
#endif

#ifndef uchar 
	typedef unsigned char   	uchar;
#endif

#ifndef llong
	typedef int64_t	    		llong;
	typedef int64_t	    		LLONG;
#endif

#ifndef u_int
	typedef uint64_t 			u_llong;
	typedef unsigned int		u_int;
	typedef unsigned short		u_short;
	typedef unsigned char   	u_char;
#endif

#ifndef intx
    typedef int64_t 		intx;
	typedef uint64_t 		uintx;
#endif

#ifndef UINT
	typedef uint64_t			ULLONG;
	typedef unsigned int    	UINT;
	typedef unsigned short  	USHORT;
	typedef unsigned char   	UCHAR;
#endif

#ifndef DWORD
	typedef uint64_t		QWORD;
	typedef uint64_t		DDWORD;
	typedef unsigned long    DWORD;
	typedef unsigned short  WORD;
	typedef unsigned char   BYTE;
#endif

#ifndef BOOL
	typedef bool            BOOL;
#endif

#ifndef TRUE
	#define TRUE 			true
	#define FALSE			false
#endif


#endif 
