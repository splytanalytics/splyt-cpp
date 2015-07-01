SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
INCLUDE = include/
OBJECTS = $(addprefix bin/,$(notdir $(SRC:.cpp=.o)))

all: $(SRC)
	g++ -fPIC -shared -I $(INCLUDE) $^ -o lib/splyt.so

tests: all tests/test.cpp
	g++ -I $(INCLUDE) tests/test.cpp -o bin/test.o -l:lib/splyt.so
	./bin/test.o

clean:
	rm -f bin/*.o lib/*.o lib/*.so