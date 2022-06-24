cmake_minimum_required (VERSION 3.4)

set (MAJOR_VERSION 0)
set (MINOR_VERSION 1)
set (PATCH_VERSION 0)
project(SpFoundation2 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

option(OPERATING_SYSTEM "Define which operating system will be built for (WINDOWS / LINUX / MACOS / ANDROID / IOS)" "WINDOWS")
option(ARCH             "Define the processor architecture (x86 / x86_64 / armv8-64)"   "x86_64")
option(FFMPEG_ENABLED   "Enable FFMPEG" ON)

set (PROJECT_DIR  ${CMAKE_CURRENT_SOURCE_DIR})
set (LIB_DIR ${PROJECT_DIR}/vendor/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/build/lib/${ARCH})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_DIR}/build/lib/${ARCH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_DIR}/build/bin/${ARCH})
set (IS_DEBUG   ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
set (IS_RELEASE ${CMAKE_BUILD_TYPE} STREQUAL "Release")
set (IS_MSVC_COMPILER ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
set (IS_64_BITS ${ARCH} STREQUAL "x86_64" OR ${ARCH} STREQUAL "armv8-64")
set (IS_WINDOWS ${OPERATING_SYSTEM} STREQUAL "WINDOWS")
set (IS_LINUX   ${OPERATING_SYSTEM} STREQUAL "LINUX")
set (IS_MACOS   ${OPERATING_SYSTEM} STREQUAL "MACOS")
set (IS_ANDROID ${OPERATING_SYSTEM} STREQUAL "ANDROID")

set (C_VERSION 17)
set (CPP_VERSION 17)
set (CMAKE_C_STANDARD ${C_VERSION})
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD ${CPP_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if( ${IS_MSVC_COMPILER} )
	if( ${IS_DEBUG} )
		if( ${BUILD_SHARED_LIBS} )
			set (MSVC_COMPILER_FLAGS "/MDd")
		else()
			set (MSVC_COMPILER_FLAGS "/MTd")
		endif()
	else()
		if( ${BUILD_SHARED_LIBS} )
			set (MSVC_COMPILER_FLAGS "/MD")
		else()
			set (MSVC_COMPILER_FLAGS "/MT")
		endif()
	endif()
endif()

if ( ${IS_64_BITS} )

	if( ${IS_MSVC_COMPILER} )
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /std:c${C_VERSION}     ${MSVC_COMPILER_FLAGS}" )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION} ${MSVC_COMPILER_FLAGS}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION} ${MSVC_COMPILER_FLAGS}" )
	else()
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m64 -fPIC -std=c${C_VERSION}"     )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -fPIC -std=c++${CPP_VERSION}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} -m64 -fPIC -std=c++${CPP_VERSION}" )

	endif()

	list(APPEND CUSTOM_DEFINITIONS "-DENV_64BITS")
else()

	if( ${IS_MSVC_COMPILER} )
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /std:c${C_VERSION}     ${MSVC_COMPILER_FLAGS}" )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION} ${MSVC_COMPILER_FLAGS}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION} ${MSVC_COMPILER_FLAGS}" )
	else()
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m32 -fPIC -std=c${C_VERSION}"     )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC -std=c++${CPP_VERSION}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} -m32 -fPIC -std=c++${CPP_VERSION}" )
	endif()

	list(APPEND CUSTOM_DEFINITIONS "-DENV_32BITS")
endif()

if ( ${IS_WINDOWS} )
	list(APPEND CUSTOM_DEFINITIONS "-DWINDOWS")
	list(APPEND CUSTOM_DEFINITIONS "-D_CRT_SECURE_NO_WARNINGS")

elseif ( ${IS_LINUX} )
	list(APPEND CUSTOM_DEFINITIONS "-DLINUX")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

elseif ( ${IS_MACOS} )
	list(APPEND CUSTOM_DEFINITIONS "-DMACOS")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

elseif ( ${IS_ANDROID} )
	list(APPEND CUSTOM_DEFINITIONS "-DANDROID")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

else()
    message(FATAL_ERROR "Operating System not defined!")
endif()

if ( ${IS_DEBUG} )
	list(APPEND CUSTOM_DEFINITIONS "-DDEBUG -DSP_ASSERT_ENABLED")
endif()

if ( ${IS_RELEASE} )
	list(APPEND CUSTOM_DEFINITIONS "-DRELEASE")
endif()

if ( ${FFMPEG_ENABLED} )
	list(APPEND CUSTOM_DEFINITIONS "-DFFMPEG_ENABLED")
endif()

file(GLOB HEADERS 
	"src/*.h"
	"src/*.hpp"
)
file(GLOB SOURCES 
	"src/*.cc"
	"src/*.cpp"
)

include_directories(
	src/
	${PROJECT_DIR}/vendor/include/
)

link_directories(
	${LIB_DIR}
)

if( ${BUILD_SHARED_LIBS} )
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

list(JOIN CUSTOM_DEFINITIONS " " CUSTOM_DEFINITIONS)
message(STATUS "FLAGS: " ${CUSTOM_DEFINITIONS})
add_definitions( ${CUSTOM_DEFINITIONS} )

if ( ${FFMPEG_ENABLED} )
	target_link_libraries(${PROJECT_NAME}
		avformat.lib
		swscale.lib
		avutil.lib
		avcodec.lib
	)
endif()