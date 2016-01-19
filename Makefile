CRF_PATH = ../crf/CRF++-0.58/

INCLUDEDIR= \
		 -I/usr/local/include \
		 -I/opt/local/include \
         -I$(CRF_PATH) \
         -I./common \
		 -I./basenlp	\
         -I./lex-align \
         -I./core

LIBDIR=	\
		/usr/local/lib/libboost_regex-mt.dylib \
		/usr/local/lib/libjsoncpp.dylib \
		/opt/local/lib/libglog.dylib \
		/usr/local/lib/libboost_filesystem.dylib \
		/usr/local/lib/libboost_system.dylib \
        $(CRF_PATH)/.libs/libcrfpp.dylib \
		-lcrypto -lpthread -lm -lstdc++

TEST = ./test
GCC = g++
CPPFLAGS = -g -O2 -Wall -Winline -pipe -ffast-math -std=c++11

SRCWILD = $(wildcard basenlp/*.cpp core/*.cpp common/*.cpp lex-align/*.cpp)
OBJs = $(patsubst %.cpp, %_at.o, $(SRCWILD))

DEMO = parser_test 
UNITTEST = unit_test

all: clean $(DEMO) $(UNITTEST) output
    #@echo $(OBJs)

$(DEMO) : parser_test.o $(OBJs)
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

parser_test.o : test/parser_test.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

$(UNITTEST) : unit_test.o $(OBJs)
	$(GCC) -g -o $@ $^ $(LIBDIR) $(INCLUDEDIR)

unit_test.o : test/unit_test.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

#%_at.o	: $(SRC)/%.cpp
%_at.o	: %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDEDIR)

output:
	#rm -fr *.o

clean:
	rm -rf *.o
	rm -rf $(OBJs)
	rm -rf core.*
	rm -rf $(DEMO)
	rm -rf $(UNITTEST)

