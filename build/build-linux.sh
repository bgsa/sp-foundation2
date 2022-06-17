#!/bin/bash

source ../build-base-linux.sh

build()
{
	make_build_dir
	
	cd $BUILD_DIR

	cmake ../../ -G "Unix Makefiles"                                      \
		-DOPERATING_SYSTEM:STRING=LINUX             \
		-DARCH:STRING=$1                                    \
		-DCMAKE_BUILD_TYPE:STRING=$2                     \
		-DBUILD_SHARED_LIBS:BOOL=$3                        \
		-DCMAKE_ENABLE_EXPORTS:BOOL=ON

	cmake --build . --config $2

	cd ../

	clear_build_dir
}

build x86_64 Debug   ON
build x86_64 Release ON