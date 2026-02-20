file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src.zip" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")

# SFML
message(STATUS "[SFML]")
add_subdirectory(src/SFML)

# clib
message(STATUS "[clib]")
add_subdirectory(src/clib/lib)