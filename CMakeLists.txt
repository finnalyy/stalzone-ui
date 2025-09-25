cmake_minimum_required(VERSION 3.16)

project(SK_PROJECT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(_MBCS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release)

file(GLOB_RECURSE SRC_FILES
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)


add_executable(loader WIN32 ${SRC_FILES})
# x64 libs
file(GLOB DIRECTX_LIBS PRIVATE
  "${PROJECT_SOURCE_DIR}/src/SDK/DirectX/Lib/x64/*.lib"
)

# -----------------

target_include_directories(loader PRIVATE
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/SDK/DirectX/Include"
  "${PROJECT_SOURCE_DIR}/src/UI/Drawing"
  "${PROJECT_SOURCE_DIR}/src/Utils"
	"${PROJECT_SOURCE_DIR}/src/SDK/ImGui"
  "${PROJECT_SOURCE_DIR}/src/UI"
)


target_link_libraries(loader PRIVATE ${DIRECTX_LIBS})
target_precompile_headers(loader PRIVATE src/pch.h)
target_link_libraries(loader PRIVATE d3d11 dxgi d3dcompiler)
