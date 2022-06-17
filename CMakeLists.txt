cmake_minimum_required (VERSION 3.3)

set (MAJOR_VERSION 0)
set (MINOR_VERSION 1)
set (PATCH_VERSION 0)
project(SpFoundation2 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

option(OPERATING_SYSTEM "Define which operating system will be built for (WINDOWS / LINUX / MACOS / ANDROID / IOS )" "WINDOWS")
option(ARCH             "Define the processor architecture (x86 / x86_64 / armv8-64 )"   "x86_64")
option(FFMPEG_ENABLED   "Enable FFMPEG" OFF)

set (PROJECT_DIR  ${CMAKE_CURRENT_SOURCE_DIR})
set (LIB_DIR ${PROJECT_DIR}/vendor/lib/${ARCH}/${CMAKE_BUILD_TYPE})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/build/lib/${ARCH})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_DIR}/build/lib/${ARCH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_DIR}/build/lib/${ARCH})

set (C_VERSION 17)
set (CPP_VERSION 17)
set (CMAKE_C_STANDARD ${C_VERSION})
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD ${CPP_VERSION})
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if ( ${ARCH} STREQUAL "x86_64" OR ${ARCH} STREQUAL "armv8-64" )

	if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /std:c${C_VERSION}"   )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION}" )
	else()
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m64 -fPIC -std=c${C_VERSION}"   )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -fPIC -std=c++${CPP_VERSION}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} -m64 -fPIC -std=c++${CPP_VERSION}" )
	endif()

	list(APPEND CUSTOM_DEFINITIONS "-DENV_64BITS")
else()

	if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /std:c${C_VERSION}"   )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} /std:c++${CPP_VERSION}" )
	else()
		set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m32 -fPIC -std=c${C_VERSION}"   )
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC -std=c++${CPP_VERSION}" )
		set (CMAKE_LDFLAGS   "${CMAKE_CXX_FLAGS} -m32 -fPIC -std=c++${CPP_VERSION}" )
	endif()

	list(APPEND CUSTOM_DEFINITIONS "-DENV_32BITS")
endif()

list(APPEND CUSTOM_DEFINITIONS "-DAPI_EXPORT")

if ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )

	if( ${BUILD_SHARED_LIBS} )
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	endif()

	list(APPEND CUSTOM_DEFINITIONS "-DWINDOWS")
	list(APPEND CUSTOM_DEFINITIONS "-D_CRT_SECURE_NO_WARNINGS")

elseif ( ${OPERATING_SYSTEM} STREQUAL "LINUX")
	list(APPEND CUSTOM_DEFINITIONS "-DLINUX")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

elseif ( ${OPERATING_SYSTEM} STREQUAL "MACOS" )
	list(APPEND CUSTOM_DEFINITIONS "-DMACOS")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

elseif ( ${OPERATING_SYSTEM} STREQUAL "ANDROID" )
	list(APPEND CUSTOM_DEFINITIONS "-DANDROID")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

else()
    message(FATAL_ERROR "Operating System not defined!")
endif()

if ( ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
	list(APPEND CUSTOM_DEFINITIONS "-DDEBUG")
endif()

if ( ${CMAKE_BUILD_TYPE} STREQUAL "Release" )
	list(APPEND CUSTOM_DEFINITIONS "-DRELEASE")
endif()

if ( ${FFMPEG_ENABLED} )
	list(APPEND CUSTOM_DEFINITIONS "-DFFMPEG_ENABLED")
	find_package(FFMPEG REQUIRED)
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
	/usr/local/include
	${SOLUTION_DIR}/vendor/include
	src
)

link_directories(
	${LIB_DIR}
#	/usr/local/lib/
)

if( ${BUILD_SHARED_LIBS} )
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

list(JOIN CUSTOM_DEFINITIONS " " CUSTOM_DEFINITIONS)
message(STATUS "FLAGS: " ${CUSTOM_DEFINITIONS})
add_definitions( ${CUSTOM_DEFINITIONS} )

target_link_libraries(${PROJECT_NAME}
    ${FFMPEG_LIBRARY}
)
