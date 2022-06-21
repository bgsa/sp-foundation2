#!/bin/bash

set echo off

source build-base.sh

make_build_dir
cd $BUILD_DIR

cmake ../../ -G "Unix Makefiles"                                      \
	-DOPERATING_SYSTEM:STRING=LINUX             \
	-DARCH:STRING=x86_64                                    \
	-DCMAKE_BUILD_TYPE:STRING=Debug                     \
	-DBUILD_SHARED_LIBS:BOOL=ON                        \
	-DCMAKE_ENABLE_EXPORTS:BOOL=ON

cmake --build . --config $2

cd ../

# clear_build_dir
