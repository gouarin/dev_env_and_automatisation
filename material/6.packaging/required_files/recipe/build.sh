#!/bin/bash

cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_BUILD_TYPE=Release -S . -G Ninja -B build
ninja -C build install
