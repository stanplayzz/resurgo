project(${package_name}-lib VERSION ${build_version})

add_library(${PROJECT_NAME})
add_library(${package_name}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC
  sfml-graphics sfml-window sfml-system clib::lib
)

target_compile_features(${PROJECT_NAME} PUBLIC
  cxx_std_23
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/")

set(version_header "${CMAKE_CURRENT_BINARY_DIR}/include/${package_name}/build_version.hpp")
configure_file(src/build_version.hpp.in "${version_header}" @ONLY)

# Collect headers
file(GLOB_RECURSE headers LIST_DIRECTORIES false "include/*.hpp")

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS
  BASE_DIRS
    include
    "${CMAKE_CURRENT_BINARY_DIR}/include"
  FILES
    ${headers}
    "${version_header}"
)

# Collect sources
file(GLOB_RECURSE sources LIST_DIRECTORIES false "src/*.[hc]pp")
target_sources(${PROJECT_NAME} PRIVATE
  ${sources}
)