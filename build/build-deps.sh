#!/bin/bash

set echo off

source build-base.sh

clear_dir  ../vendor/
create_dir ../vendor/

cd ../vendor/

git clone git@github.com:FFmpeg/FFmpeg.git

cd FFmpeg

git checkout tags/n5.0.1 -b n5.0.1
git pull

./configure \
  --arch=x86_64 \
  --prefix="output/ffmpeg_build" \
  --extra-libs="-lpthread -lm" \
  --bindir="output/bin"
  --disable-x86asm
  --enable-shared \
  --disable-x86asm \
  --disable-programs \
  --disable-doc \
  --disable-swresample \
  --disable-postproc \
  --disable-avfilter \
  --disable-w32threads \
  --disable-dct \
  --disable-dwt \
  --disable-lsp  \
  --disable-mdct \
  --disable-rdft \
  --disable-fft \
  --disable-faan \
  --disable-bzlib

make
make install

cd ../../build/
