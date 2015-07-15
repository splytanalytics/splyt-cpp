ifndef CXX
CXX = g++
endif
SRC = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard vendor/*/*.cc) $(wildcard vendor/*/*.cpp)
INCLUDE = -I src/ -I vendor/jsoncpp
PARAM = -std=c++0x
OBJECTS = $(addprefix bin/,$(notdir $(SRC:.cc=.o)))
LIB_PATH = $(shell pwd)/lib

$(shell mkdir -p lib)
$(shell mkdir -p bin)

all:

unix: $(SRC)
	$(CXX) $(PARAM) -fPIC -shared $(INCLUDE) $^ -o lib/libsplyt.so -lcurl

win: $(SRC)
	$(CXX) $(PARAM) -shared $(INCLUDE) $^ -o lib/libsplyt.dll -lcurl
	cp lib/libsplyt.dll bin/libsplyt.dll

unix-tests: unix tests/test.cc
	$(CXX) $(INCLUDE) tests/test.cc -o bin/test.o -Llib -lsplyt
	export LD_LIBRARY_PATH="$(LIB_PATH)"; ./bin/test.o

win-tests: win tests/test.cc
	$(CXX) $(INCLUDE) tests/test.cc -o bin/test.exe -Llib -lsplyt
	export LD_LIBRARY_PATH="$(LIB_PATH)"; ./bin/test.exe

clean:
	rm -f bin/* lib/*
