#!/bin/bash

cd .. && mkdir -p cmake/build && cd cmake/build

cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS_RELEASE="-O3 -flto -march=native" \
    -DCMAKE_C_FLAGS_RELEASE="-O3 -flto -march=native" \
    -DCMAKE_C_COMPILER=clang-17 \
    -DCMAKE_CXX_COMPILER=clang++-17 \
    ../..

make -j$(nproc)

