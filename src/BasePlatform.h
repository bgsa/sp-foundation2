#ifndef BASE_PLATFORM_HEADER
#define BASE_PLATFORM_HEADER

#define HEAP_PROFILING_ALLOC
#define DEPRECATED

#define PRAGMA(content) _Pragma(#content)

#ifdef __cplusplus 
	#define EXTERN_C extern "C"
#else
	#define EXTERN_C
#endif

#if defined (_MSC_VER)
	#include <codeanalysis\warnings.h>

	#define MSVC_COMPILER
	#define COMPILER_VERSION _MSC_FULL_VER
	#define UNINITIALIZED_HEAP_ADDRESS 0xCDCDCDCD
	#define UNINITIALIZED_STACK_ADDRESS 0xCCCCCCCC
	#define FREED_MEMORY_ADDRESS 0xDDDDDDDD

	#if defined(_M_X64) || defined(_M_AMD64)
		#ifndef ENV_64BITS
			#define ENV_64BITS
		#endif
		#ifdef ENV_32BITS
			#undef ENV_32BITS
		#endif
	#else
		#ifndef ENV_32BITS
			#define ENV_32BITS
		#endif
		#ifdef ENV_3264TS
			#undef ENV_64BITS
		#endif
	#endif

	#if defined(_M_ARM) || defined(_M_ARM_ARMV7VE) || defined(_M_ARM_FP) || defined(_M_ARM64)
		#define ARM_PROCESSOR
	#elif defined(_M_IX86) || defined(_M_IA64)
		#define INTEL_PROCESSOR
	#endif

	#define SP_PUSH_WARNING          PRAGMA( warning push )
	#define SP_ENABLE_WARNING        PRAGMA( warning pop )
	#define SP_DISABLE_WARNING(code) \
		SP_PUSH_WARNING          \
		PRAGMA( warning disable : code )
	#define SP_DISABLE_WARNING_IGNORED_ATTRIBUTES SP_DISABLE_WARNING(check the warning MSVC code)

	#if defined(DEBUG)
		//#define _CRTDBG_MAP_ALLOC
		//#undef free

		#undef HEAP_PROFILING_ALLOC
		#define HEAP_PROFILING_ALLOC __declspec(allocator)
	#endif

	#undef  DEPRECATED
	#define DEPRECATED __declspec(deprecated)

	//	Type Traits compile definitions
	#define isPrimitive(Type) !(__is_class(Type))
	#define isClass(Type) __is_class(Type)
	#define isAbstractClass(Type) __is_abstract(Type)
	#define isEnum(Type) __is_enum(Type)
	#define isConvertibleTo(from,to) __is_convertible_to(from, to)
	#define isDerivedOf(Base,Derived) __is_base_of(Base, Derived)

#elif defined (__GNUC__)
	#define GCC_COMPILER
	#define COMPILER_VERSION __GNUC___.__GNUC_MINOR__
	#define UNINITIALIZED_HEAP_ADDRESS 0x00000000

	#define SP_PUSH_WARNING          PRAGMA( GCC diagnostic push )
	#define SP_ENABLE_WARNING        PRAGMA( GCC diagnostic pop )
	#define SP_DISABLE_WARNING(name)  \
		SP_PUSH_WARNING           \
		PRAGMA( GCC diagnostic ignored name )
	#define SP_DISABLE_WARNING_IGNORED_ATTRIBUTES SP_DISABLE_WARNING("-Wignored-attributes")

	#undef  DEPRECATED
	#define DEPRECATED __attribute__((deprecated))

	#if defined(__x86_64__)

		#ifndef ENV_64BITS
			#define ENV_64BITS
		#endif

		#ifdef ENV_32BITS
			#undef ENV_32BITS
		#endif

		#if defined(__ia64__) || defined(_IA64) || defined(__IA64__)
			#define INTEL_PROCESSOR
		#endif
	#else
	
		#ifndef ENV_32BITS
			#define ENV_32BITS
		#endif

		#ifdef ENV_64BITS
			#undef ENV_64BITS
		#endif

		#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
			#define INTEL_PROCESSOR
		#endif
	#endif

	//	Type Traits compile definitions
	#define isPrimitive(Type) !(__is_class(Type))
	#define isClass(Type) __is_class(Type)
	#define isAbstractClass(Type) __is_abstract(Type)
	#define isEnum(Type) __is_enum(Type)
	#define isConvertibleTo(from,to) __is_convertible_to(from, to)
	#define isDerivedOf(Base,Derived) __is_base_of(Base, Derived)

#elif defined (__clang__)
	#define CLANG_COMPILER
	#define COMPILER_VERSION __clang_major__.__clang_minor__.__clang_patchlevel__
	#define UNINITIALIZED_HEAP_ADDRESS 0x00000000

	#undef  DEPRECATED
	#define DEPRECATED __attribute__((deprecated))

	#define SP_PUSH_WARNING          PRAGMA( clang diagnostic push )
	#define SP_ENABLE_WARNING        PRAGMA( clang diagnostic pop )
	#define SP_DISABLE_WARNING(name)  \
		SP_PUSH_WARNING           \
		PRAGMA( clang diagnostic ignored name )
	#define SP_DISABLE_WARNING_IGNORED_ATTRIBUTES SP_DISABLE_WARNING("-Wignored-attributes")

	#if defined(__x86_64__)

		#ifdef ENV_32BITS
			#undef ENV_32BITS
		#endif

		#ifndef ENV_64BITS
			#define ENV_64BITS
		#endif

	#else

		#ifdef ENV_64BITS
			#undef ENV_64BITS
		#endif

		#ifndef ENV_32BITS
			#define ENV_32BITS
		#endif

	#endif

	#if defined(__has_extension(attribute_deprecated_with_message))
		#undef  DEPRECATED
		#define DEPRECATED __attribute__((deprecated("Deprecated!")))
	#endif

//__has_feature(cxx_constexpr)
//__has_feature(cxx_exceptions)
//__has_feature(cxx_rtti)

	//	Type Traits compile definitions
	#define isPrimitive(Type) !(__is_class(Type))
	#define isClass(Type) __is_class(Type)
	#define isAbstractClass(Type) __is_abstract(Type)
	#define isEnum(Type) __is_enum(Type)
	#define isConvertibleTo(from,to) __is_convertible_to(from, to)
	#define isDerivedOf(Base,Derived) __is_base_of(Base, Derived)

#elif defined (__MINGW32__)
	#define MINGW32_COMPILER
	#define COMPILER_VERSION __MINGW32_MAJOR_VERSION.__MINGW32_MINOR_VERSION

	#if defined(_X86_)
		#define INTEL_PROCESOR
	#endif

#elif defined (__MINGW64__)
	#define MINGW64_COMPILER
	#define COMPILER_VERSION __MINGW64_VERSION_MAJOR.__MINGW64_VERSION_MINOR

#elif defined(__wasm__)
	#define WASM_COMPILER
	#define COMPILER_VERSION 0

#elif defined(__CC_ARM)
	#define ARM_COMPILER
	#define COMPILER_VERSION __ARMCC_VERSION

#elif defined(__INTEL_COMPILER)
	#define INTEL_COMPILER
	#define COMPILER_VERSION __INTEL_COMPILER

	#undef  DEPRECATED
	#define DEPRECATED [[deprecated]]
#endif

#if defined(__CUDACC__)
	#define CUDA_ENABLED
//CUDA_VERSION
#endif



// Define the Operating System
#if defined (_WIN32) || defined (_WIN64)
	#if !defined(WINDOWS)
		#define WINDOWS
	#endif
#elif defined(__linux__) && !defined(__ANDROID__)
	#if !defined(LINUX)
		#define LINUX
	#endif
#elif defined (__ANDROID__)
	#if !defined(ANDROID)
		#define ANDROID
	#endif
	//__ANDROID_API__
#elif defined (__APPLE__) // MacOS ou iOS
	#if !defined(OSX)
		#define OSX
	#endif
#endif

/* Define Processor Architecture
__i386__
__x86_64__
__arm__. If defined, you can further check:
	__ARM_ARCH_5T__
	__ARM_ARCH_7A__
	__powerpc64__
	__aarch64__
*/


/* Define the C++ libraries
__GLIBCXX__	       libstdc++
__GLIBCPP__
__INTEL_CXXLIB_ICC


_LIBCPP_VERSION	   libc++
_MSC_VER	       MSVC

__GLIBC__	       GNU glibc
<features.h>
__GNU_LIBRARY__

__BIONIC__	       Bionic libc
<sys/types.h>

__INTEL_CXXLIB_ICC
*/

#include <limits.h>

#define SP_CONSTEXPR constexpr

#ifdef ENV_32BITS
	#define SIZEOF_WORD              (4)
	#define SIZEOF_TWO_WORDS         (8)
#else
	#define SIZEOF_WORD              (8)
	#define SIZEOF_TWO_WORDS         (16)
#endif

#define SP_SHORT_MAX        SHRT_MAX
#define SP_USHORT_MAX       USHRT_MAX
#define SP_INT_MAX          INT_MAX
#define SP_UINT_MAX         UINT_MAX
#define SP_FLOAT_MAX        FLT_MAX
#define SP_DOUBLE_MAX       DBL_MAX

#define SP_FLOAT_MIN        (-FLT_MAX)
#define SP_SHORT_MIN        (-SHRT_MAX)
#define SP_USHORT_MIN       (-USHRT_MAX)
#define SP_INT_MIN          (-INT_MAX)
#define SP_UINT_MIN         (-UINT_MAX)
#define SP_FLOAT_MIN        (-FLT_MAX)
#define SP_DOUBLE_MIN       (-DBL_MAX)

#ifdef ENV_32BITS
	#define SP_SIZE_MAX      UINT32_MAX
	#define SP_SIZE_MIN      (-SP_SIZE_MAX)
#else
	#define SP_SIZE_MAX      UINT64_MAX
	#define SP_SIZE_MIN      (-SP_SIZE_MAX)
#endif

#define SP_HUGE_VALUE_FLOAT ((sp_float) 1e50)
#define SP_INFINITY         SP_HUGE_VALUE_FLOAT

#define ONE_KILOBYTE sizeof(sp_char) * 1024
#define ONE_MEGABYTE ONE_KILOBYTE * 1024
#define ONE_GIGABYTE ONE_MEGABYTE * 1024

#define EULER_NUMBER (2.71828f)   // e^1 = 2.71828
#define PI (3.1415926f)
#define TWO_PI (6.2831853f)
#define HALF_PI (1.5707963f)
#define PI_DIV_180 (0.0174532f)
#define INV_PI_DIV_180 (57.2957795f)
#define	PI_DIV_360 (0.0087266f)

#define DECIMAL_BASE (10)

#define MAX_DIGITS_EXPOENT  (5)
#define MAX_DIGITS_MANTISSA (4)

#define divideBy(value, bit) ((value) >> bit)
#define divideBy2(value) divideBy(value, SHIFT_BIT_ONE)
#define divideBy4(value) ((value) >> SHIFT_BIT_TWO)
#define divideBy8(value) ((value) >> SHIFT_BIT_THREE)
#define divideBy16(value) ((value) >> SHIFT_BIT_FOUR)

#define multiplyBy(value, bit) ((value) << bit)
#define multiplyBy2(value) multiplyBy(value, SHIFT_BIT_ONE)
#define multiplyBy3(value) (multiplyBy2(value) + value)
#define multiplyBy4(value) multiplyBy(value, SHIFT_BIT_TWO)
#define multiplyBy5(value) (multiplyBy4(value) + value)
#define multiplyBy6(value) (multiplyBy4(value) + multiplyBy2(value))
#define multiplyBy8(value) multiplyBy(value, SHIFT_BIT_THREE)
#define multiplyBy9(value) (multiplyBy8(value) + value)
#define multiplyBy10(value) (multiplyBy8(value) + multiplyBy2(value))
#define multiplyBy12(value) (multiplyBy8(value) + multiplyBy4(value))
#define multiplyBy16(value) multiplyBy(value, SHIFT_BIT_FOUR)

#ifdef ENV_32BITS
	#define SIZEOF_WORD_DIVISOR_BIT (2)

	#define sp_ceilBit(value, divisor)      \
		((value % divisor == ZERO_DOUBLE) ? \
			divideBy(value, 2)              \
		:                                   \
			divideBy(value, 2) + 1)
#else
	#define SIZEOF_WORD_DIVISOR_BIT (3)

	#define sp_ceilBit(value, divisor)      \
		((value % divisor == ZERO_DOUBLE) ? \
			divideBy(value, 3)              \
		:                                   \
			divideBy(value, 3) + 1)
#endif

#define sp_isHeapInitialized(value) \
	value != UNINITIALIZED_HEAP_ADDRESS

#define sp_isStackInitialized(value) \
	value != UNINITIALIZED_STACK_ADDRESS

#define isCppGreatherThanCpp11 (__STDC_VERSION__ > 201400L)
#define isCppGreatherThanCpp14 (__STDC_VERSION__ > 201700L)

extern "C" {
	typedef unsigned char  sp_byte;  // 1byte
	typedef unsigned short sp_word;  // 2bytes
	typedef unsigned long  sp_dword; // 4bytes

	const char END_OF_LINE_CR = '\r';
	const char END_OF_LINE_LF = '\n';
	const char END_OF_LINE_CRLF[2] = { '\r', '\n' };
	
	const char END_OF_LINE = END_OF_LINE_LF;
	const char END_OF_STRING = '\0';

	const char SP_STRING_SPACE = ' ';
}

#endif // BASE_PLATFORM_HEADER
