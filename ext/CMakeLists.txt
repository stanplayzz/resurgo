file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src.zip" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")

# GLFW
message(STATUS "[GLFW]")
add_subdirectory(src/glfw)

# glad
message(STATUS "[glad]")
add_library(glad STATIC 
    src/glad/src/glad.c
)
target_include_directories(glad PUBLIC 
    src/glad/include
)

# glm
message(STATUS "[glm]")
add_subdirectory(src/glm)

# clib
message(STATUS "[clib]")
add_subdirectory(src/clib)

# freetype
message(STATUS "[freetype]")
add_subdirectory(src/freetype)

# stb image
message(STATUS "[stb_image.h]")
add_library(stb_image STATIC src/stb_image.cpp)
target_include_directories(stb_image PUBLIC 
    src/
)

target_compile_options(stb_image PRIVATE
    -w 
)