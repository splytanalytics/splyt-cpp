SRC = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard vendor/*/*.cc) $(wildcard vendor/*/*.cpp)
INCLUDE = -I src/ -I vendor/jsoncpp
PARAM = -std=c++11
OBJECTS = $(addprefix bin/,$(notdir $(SRC:.cc=.o)))
LIB_PATH = $(shell pwd)/lib

$(shell mkdir -p lib)
$(shell mkdir -p bin)

all:

lin: $(SRC)
	g++ $(PARAM) -fPIC -shared  $(INCLUDE) $^ -o lib/libsplyt.so

mac: $(SRC)
	g++ $(PARAM) -fPIC -undefined suppress -flat_namespace $(INCLUDE) $^ -o lib/libsplyt.dylib

win: $(SRC)
	g++ $(PARAM) -shared $(INCLUDE) $^ -o lib/libsplyt.dll
	cp lib/libsplyt.dll bin/libsplyt.dll

lin-tests: lin tests/test.cc
	g++ $(INCLUDE) tests/test.cc -o bin/test.o -Llib -lsplyt -lcurl
	export LD_LIBRARY_PATH="$(LIB_PATH)"; ./bin/test.o

mac-tests: mac tests/test.cc
	g++ $(INCLUDE) tests/test.cc -o bin/test.o -Llib -lsplyt -lcurl
	export LD_LIBRARY_PATH="$(LIB_PATH)"; ./bin/test.o

win-tests: win tests/test.cc
	g++ $(INCLUDE) tests/test.cc -o bin/test.exe -Llib -lsplyt -lcurl
	export LD_LIBRARY_PATH="$(LIB_PATH)"; ./bin/test.exe

clean:
	rm -f bin/* lib/*