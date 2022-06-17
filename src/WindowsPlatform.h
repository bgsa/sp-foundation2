#ifdef WINDOWS

#ifndef WINDOWS_PLATFORM_HEADER
#define WINDOWS_PLATFORM_HEADER

# ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
# endif
#define NOMINMAX
#include <Windows.h>
#undef near
#undef far

#ifdef API_IMPORT
	#define API_INTERFACE __declspec(dllimport)		
#else
	#ifdef API_EXPORT
		#define API_INTERFACE __declspec(dllexport)
	#else
		#define API_INTERFACE
	#endif
#endif

#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl

#ifdef ENV_32BITS
	#define SHIFT_BIT_ONE   1
	#define SHIFT_BIT_TWO   2
	#define SHIFT_BIT_THREE 3
	#define SHIFT_BIT_FOUR  4
#elif defined(ENV_64BITS)
	#define SHIFT_BIT_ONE   1i64
	#define SHIFT_BIT_TWO   2i64
	#define SHIFT_BIT_THREE 3i64
	#define SHIFT_BIT_FOUR  4i64
#else
	#error "Environment not 32 or 64-bit"
#endif

typedef bool    sp_bool;
typedef char    sp_char;
typedef wchar_t sp_wchar;
typedef float   sp_float;
typedef double  sp_double;

typedef signed __int8    sp_int8;
typedef signed __int16   sp_short;
typedef signed __int16   sp_int16;
typedef signed __int32   sp_int;
typedef signed __int32   sp_int32;
typedef signed __int64   sp_long;
typedef signed __int64   sp_int64;
typedef signed long long sp_longlong;

typedef unsigned __int8     sp_uint8;
typedef unsigned __int8     sp_byte;
typedef unsigned __int8     sp_uchar;
typedef unsigned __int16    sp_uint16;
typedef unsigned __int16    sp_ushort;
typedef unsigned __int16    sp_half;
typedef unsigned __int32    sp_uint32;
typedef unsigned __int32    sp_uint;
typedef unsigned __int64    sp_uint64;
typedef unsigned __int64    sp_ulong;
typedef unsigned long long  sp_long_size;

#ifdef ENV_32BITS
typedef unsigned int     sp_size;
//	typedef int              ptrdiff_t;
//	typedef int              intptr_t;
#else
typedef unsigned __int64 sp_size;
//	typedef __int64          ptrdiff_t;
//	typedef __int64          intptr_t;
#endif

#define ZERO_SHORT   0i16
#define ONE_SHORT    1i16
#define TWO_SHORT    2i16
#define THREE_SHORT  3i16
#define FOUR_SHORT   4i16
#define TEN_SHORT   10i16

#define ZERO_USHORT   0ui16
#define ONE_USHORT    1ui16
#define TWO_USHORT    2ui16
#define THREE_USHORT  3ui16
#define FOUR_USHORT   4ui16
#define TEN_USHORT   10ui16

#define ZERO_INT   0i32
#define ONE_INT    1i32
#define TWO_INT    2i32
#define THREE_INT  3i32
#define FOUR_INT   4i32
#define TEN_INT   10i32

#define ZERO_UINT   0ui32
#define ONE_UINT    1ui32
#define TWO_UINT    2ui32
#define THREE_UINT  3ui32
#define FOUR_UINT   4ui32
#define TEN_UINT   10ui32

#define ZERO_LONG   0i64
#define ONE_LONG    1i64
#define TWO_LONG    2i64
#define TRHEE_LONG  3i64
#define FOUR_LONG   4i64
#define TEN_LONG   10i64

#define ZERO_ULONG   0ui64
#define ONE_ULONG    1ui64
#define TWO_ULONG    2ui64
#define THREE_ULONG  3ui64
#define FOUR_ULONG   4ui64
#define TEN_ULONG   10ui64

#ifdef ENV_32BITS
	#define ZERO_SIZE   0ui32
	#define ONE_SIZE    1ui32
	#define TWO_SIZE    2ui32
	#define THREE_SIZE  3ui32
	#define FOUR_SIZE   4ui32
	#define TEN_SIZE   10ui32
#else
	#define ZERO_SIZE   0ui64
	#define ONE_SIZE    1ui64
	#define TWO_SIZE    2ui64
	#define THREE_SIZE  3ui64
	#define FOUR_SIZE   4ui64
	#define TEN_SIZE   10ui64
#endif

#define ZERO_FLOAT  0.0f
#define HALF_FLOAT  0.5f
#define ONE_FLOAT   1.0f
#define TWO_FLOAT   2.0f
#define THREE_FLOAT 3.0f
#define FOUT_FLOAT  4.0f
#define TEN_FLOAT  10.0f

#define ZERO_DOUBLE  0.0
#define HALF_DOUBLE  0.5
#define ONE_DOUBLE   1.0
#define TWO_DOUBLE   2.0
#define THREE_DOUBLE 3.0
#define FOUR_DOUBLE  4.0
#define TEN_DOUBLE  10.0

#define SP_NOT_A_NUMBER     (INFINITY - INFINITY)

#define SP_DIRECTORY_SEPARATOR     '\\'
#define SP_DIRECTORY_SEPARATOR_STR "\\"
#define SP_DIRECTORY_SEPARATOR_LENGTH (2)

inline void getLastErrorMessage(sp_char* message, const sp_size messageSize)
{
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		GetLastError(),
		LOCALE_USER_DEFAULT,
		message,
		(DWORD)messageSize,
		nullptr);
}

#endif // WINDOWS_PLATFORM_HEADER

#endif // WINDOWS