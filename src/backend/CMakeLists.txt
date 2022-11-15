set(SRC token.c lexer.c syntax_check.c convert.c parser.c)
add_library(Interpreter ${SRC})

# Allow users of Interpreter to also include its headers, hence PUBLIC
target_include_directories(Interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

# add -Wall and -Wextra
# also add them to modules using this library, hence PUBLIC
target_compile_options(Interpreter PUBLIC -Wall -Wextra)

# link the math library
target_link_libraries(Interpreter PRIVATE m)
