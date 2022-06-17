#ifndef ASSERTIONS_HEADER
#define ASSERTIONS_HEADER

#if defined(DEBUG) && defined(SP_ASSERT_ENABLED)
	#include <cassert>
	#include <iostream>

	#define sp_assert(condition, message)											\
		if (!(condition))															\
		{																			\
			std::cout << message << ": " << std::strerror(errno) << END_OF_LINE;    \
			std::cout << "File: " << __FILE__ << " (line " << __LINE__ << ")";		\
			std::cout << END_OF_LINE;												\
		}																			\
		assert(condition)
			
#else
	#define sp_assert(condition, message)
#endif

#endif // ASSERTIONS_HEADER