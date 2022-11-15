set(SRC main.c tokenprint.c)
add_executable(Parser ${SRC})

# Parser is an executable, no need to share, hence PRIVATE
target_include_directories(Parser PRIVATE headers)

# Calls made into Parser do not refer to anything
# in Interpreter directly, hence PRIVATE
target_link_libraries(Parser PRIVATE Interpreter)

# add -Wall and -Wextra
# also add them to modules using this executable, hence PUBLIC
target_compile_options(Parser PUBLIC -Wall -Wextra)
