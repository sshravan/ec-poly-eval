include_directories(
    ${CMAKE_SOURCE_DIR}/depends/
    ${CMAKE_SOURCE_DIR}/depends/mcl
)

add_executable(
  main

  main.c
)
target_link_libraries(
  main

  mcl
  mcl::mclbn384_256
)
