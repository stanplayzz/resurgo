add_executable(${PROJECT_NAME} WIN32)

target_link_libraries(${PROJECT_NAME} PRIVATE
  resurgo::resurgo-lib
)

target_include_directories(${PROJECT_NAME} PRIVATE
  src
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)

target_sources(${PROJECT_NAME} PRIVATE
  src/main.cpp
)