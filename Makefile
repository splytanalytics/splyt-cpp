ifndef CXX
CXX = g++
endif
SRC = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard vendor/*/*.cc) $(wildcard vendor/*/*.cpp) $(wildcard platform/curl/*.cc)
BOOST_SRC = $(wildcard projects/visual-studio/vendor/boost-libs/*.cpp)
INCLUDE = -I src -I vendor -I platform/curl
PARAM = -std=c++0x
OBJECTSP1 = $(addprefix ,$(notdir $(SRC:.cc=.o)))
OBJECTS = $(addprefix ,$(notdir $(OBJECTSP1:.cpp=.o)))
BOOST_OBJECTS = $(addprefix ,$(notdir $(BOOST_SRC:.cpp=.o)))
LIB_PATH = $(shell pwd)/lib

$(shell mkdir -p lib)
$(shell mkdir -p bin)

all:

unix: $(SRC)
	$(CXX) $(PARAM) -fPIC -Wall -Wno-unknown-pragmas -shared $(INCLUDE) $^ -o lib/libsplyt.so -lcurl -lboost_system -lboost_thread

win: $(SRC) $(BOOST_SRC)
	$(CXX) $(PARAM) $(INCLUDE) -Iprojects/visual-studio/vendor -DLIBSPLYT_EXPORTS -DWIN32 -DJSON_DLL_BUILD -DBOOST_ARCHIVE_SOURCE -DBOOST_ALL_DYN_LINK -DBOOST_DLL_EXPORT -c $^ -Lprojects/visual-studio/vendor/curl -lcurl
	$(CXX) $(PARAM) -shared $(INCLUDE) -Iprojects/visual-studio/vendor -o lib/libsplyt.dll $(OBJECTS) $(BOOST_OBJECTS) -Lprojects/visual-studio/vendor/curl -lcurl -lcurldll -Wl,--out-implib,lib/libsplyt.a
	rm *.o
	cp lib/libsplyt.dll bin/libsplyt.dll

unix-tests: unix tests/test.cc
	$(CXX) $(INCLUDE) tests/test.cc -o bin/test.o -Llib -lsplyt -lboost_system -lboost_thread
	export LD_LIBRARY_PATH="$(LIB_PATH)"; ./bin/test.o

win-tests: win tests/test.cc
	$(CXX) $(INCLUDE) -Iprojects/visual-studio/vendor tests/test.cc -o bin/test.exe -Llib -lsplyt -Lprojects/visual-studio/vendor/curl -lcurl
	cp -a projects/visual-studio/vendor/curl-dlls/. bin/.
	export LD_LIBRARY_PATH="$(LIB_PATH)"; cd bin; ./test.exe

clean:
	rm *.o
	rm -f bin/* lib/*
