#!/bin/bash

set echo off

source build-base.sh

clear_dir  ../vendor/
create_dir ../vendor/

cd ../vendor/

clear_dir  include/
create_dir include/

clear_dir  lib/
create_dir lib/
create_dir lib/x86_64/

clear_dir  bin/
create_dir bin/
create_dir bin/x86_64/

source ../build/build-dep-ffmpeg.sh

cd ../build/
