cmake_minimum_required(VERSION 3.24)

set(CMAKE_BUILD_RPATH "$ORIGIN")

set(build_version 0.1.0)
set(package_name resurgo)
project(${package_name} VERSION ${build_version})


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-d")

add_subdirectory(ext)
add_subdirectory(lib)
add_subdirectory(app)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
    $<$<NOT:$<CONFIG:Debug>>:-Werror>
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<NOT:$<CONFIG:Debug>>:/WX>
  )
endif()