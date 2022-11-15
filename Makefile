.PHONY: all test config config_debug clean wipe

BUILD_DIR = build/

all: ${BUILD_DIR}/Makefile
	@cd ${BUILD_DIR}; \
		make

test: ${BUILD_DIR}/Makefile
	@cd ${BUILD_DIR}; \
		make Test; \
		./Test

${BUILD_DIR}/Makefile:
	@cd ${BUILD_DIR}; \
		cmake ..

config:
	@cd ${BUILD_DIR}; \
		cmake ..

config_debug:
	@cd ${BUILD_DIR}; \
		cmake -DCMAKE_BUILD_TYPE=Debug ..

clean:
	@cd ${BUILD_DIR}; \
		make clean

wipe:
	rm -rf ${BUILD_DIR}/*
