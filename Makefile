SRC = src/*.cpp
INCLUDE = include/

all: $(DRIVER_INC) $(DRIVER_SRC)
	gcc -fPIC -c -I $(INCLUDE) -o lib/splyt.o $(SRC)
	#gcc -shared -o lib/splyt.so lib/splyt.o

test: all test/test.cpp
	gcc -I $(INCLUDE) -l:lib/splyt.o test/test.cpp -o bin/test.o
	./bin/test.o

clean:
	rm -f bin/*.o lib/*.o lib/*.so