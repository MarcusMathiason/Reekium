#!/bin/bash

set -e 

mkdir -p build

g++ Main.cpp ShaderUtil.cpp -o build/Reekium \
  -std=c++20 \
  -O2 \
  -Wall \
  -Wextra \
  $(pkg-config --cflags --libs sdl2 glew) \
  -lGL

echo "Build complete!"
