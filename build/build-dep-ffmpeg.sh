
git clone git@github.com:FFmpeg/FFmpeg.git

cd FFmpeg

git checkout tags/n5.0.1 -b n5.0.1

./configure                          \
  --arch=x86_64                      \
  --prefix="output/ffmpeg_build"     \
  --extra-libs="-lpthread -lm"       \
  --bindir="output/bin"              \
  --disable-x86asm                   \
  --enable-shared                    \
  --disable-programs                 \
  --disable-doc                      \
  --disable-avdevice                 \
  --disable-swresample               \
  --disable-postproc                 \
  --disable-avfilter                 \
  --disable-w32threads               \
  --disable-dct                      \
  --disable-dwt                      \
  --disable-lsp                      \
  --disable-mdct                     \
  --disable-rdft                     \
  --disable-fft                      \
  --disable-faan                     \
  --disable-bzlib

make
make install

cp -R output/ffmpeg_build/include/ ../

cp output/ffmpeg_build/lib/*.so* ../lib/x86_64/

cp output/ffmpeg_build/lib/*.a ../bin/x86_64/

cd ../

#clear_dir FFmpeg/
