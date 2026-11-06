cmake -S . -B build -G Ninja %CMAKE_ARGS% -DCMAKE_INSTALL_PREFIX="%LIBRARY_PREFIX%" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit 1

cmake --build build --target install
if errorlevel 1 exit 1
