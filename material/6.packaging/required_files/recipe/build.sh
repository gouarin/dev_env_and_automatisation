#!/bin/bash
set -euxo pipefail

cmake -S . -B build -G Ninja ${CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX="${PREFIX}" -DCMAKE_BUILD_TYPE=Release
cmake --build build --target install
