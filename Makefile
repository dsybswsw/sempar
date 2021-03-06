SRC = ./src
INC = ./inc

INCLUDEDIR= \
		 -I/usr/local/include \
		 -I/opt/local/include \
		 -I./$(INC)	\

LIBDIR=	\
		/usr/local/lib/libboost_regex-mt.dylib \
		/usr/local/lib/libjsoncpp.dylib \
		/opt/local/lib/libglog.dylib \
        /usr/local/lib/libboost_filesystem.dylib \
        /usr/local/lib/libboost_system.dylib \
		-lcrypto -lpthread -lm -lstdc++

TEST = ./test
GCC = g++
CPPFLAGS = -g -O2 -Wall -Winline -pipe -ffast-math -std=c++11

SRCWILD = $(wildcard $(SRC)/*.cpp)
OBJs = $(patsubst $(SRC)/%.cpp, %_at.o, $(SRCWILD))

DEMO = parser_test 
UNITTEST = unit_test

all: clean $(DEMO) $(UNITTEST) output

$(DEMO) : parser_test.o $(OBJs)
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

parser_test.o : test/parser_test.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

$(UNITTEST) : unit_test.o $(OBJs)
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

unit_test.o : test/unit_test.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

%_at.o	: $(SRC)/%.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

output:
	#rm -fr *.o

clean:
	rm -rf *.o
	rm -rf core.*
	rm -rf $(DEMO)
	rm -rf $(UNITTEST)

