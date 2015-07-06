SRC = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard vendor/*/*.cc) $(wildcard vendor/*/*.cpp)
INCLUDE = -I src/ -I vendor/jsoncpp
OBJECTS = $(addprefix bin/,$(notdir $(SRC:.cc=.o)))

all: $(SRC)
	g++ -fPIC -shared $(INCLUDE) $^ -o lib/splyt.so

tests: all tests/test.cc
	g++ $(INCLUDE) tests/test.cc -o bin/test.o -l:lib/splyt.so -lcurl
	./bin/test.o

clean:
	rm -f bin/*.o lib/*.o lib/*.so