cmake -S . -G "Ninja" -D CMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% -D CMAKE_BUILD_TYPE=Release -B build ^
   -DOpenCV_DIR=%LIBRARY_PREFIX% ^
   -DOpenCV_LIB_PATH=%LIBRARY_LIB% ^
   -DOpenCV_INCLUDE_DIRS=%LIBRARY_INC%
if errorlevel 1 exit 1

ninja -C build install
if errorlevel 1 exit 1
