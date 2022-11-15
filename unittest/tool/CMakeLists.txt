set(SRC unittest.c)
add_library(TestTool ${SRC})

target_include_directories(TestTool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TestTool PUBLIC Interpreter)

target_compile_options(TestTool PUBLIC -Wall -Wextra)
