set(SRC test.c)
add_executable(Test ${SRC})

target_link_libraries(Test PUBLIC TestTool)

target_compile_options(Test PUBLIC -Wall -Wextra)

add_subdirectory(tool)
