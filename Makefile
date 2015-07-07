SRC = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard vendor/*/*.cc) $(wildcard vendor/*/*.cpp)
INCLUDE = -I src/ -I vendor/jsoncpp
OBJECTS = $(addprefix bin/,$(notdir $(SRC:.cc=.o)))
LIB_PATH = $(shell pwd)/lib

all: $(SRC)
	g++ -std=c++0x -fPIC -shared $(INCLUDE) $^ -o lib/libsplyt.so

tests: all tests/test.cc
	g++ $(INCLUDE) tests/test.cc -o bin/test.o -Llib -lsplyt -lcurl
	export LD_LIBRARY_PATH=$(LIB_PATH); ./bin/test.o

clean:
	rm -f bin/*.o lib/*.o lib/*.so
