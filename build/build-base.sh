#!/bin/bash

export BUILD_DIR=output
export current_dir=$(pwd)

create_dir()
{
	if [ ! -d "$1" ]; then
		mkdir -p "$1"
	fi
}

clear_dir()
{
	if [ -d $1 ]; then
		rm -rf $1
	fi	  
}

make_build_dir()
{
	clear_dir $BUILD_DIR
	create_dir $BUILD_DIR
}