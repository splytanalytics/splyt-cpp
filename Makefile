SRC = $(wildcard src/*.cc) $(wildcard src/*/*.cc)
INCLUDE = src/
OBJECTS = $(addprefix bin/,$(notdir $(SRC:.cc=.o)))

all: $(SRC)
	g++ -fPIC -shared -I $(INCLUDE) $^ -o lib/splyt.so

tests: all tests/test.cc
	g++ -I $(INCLUDE) tests/test.cc -o bin/test.o -l:lib/splyt.so -lcurl
	./bin/test.o

clean:
	rm -f bin/*.o lib/*.o lib/*.so