cmake -S . -B build -G "Visual Studio 18 2026" -A x64 ^ -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
