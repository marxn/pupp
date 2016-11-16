INCLUDE = -I../include -I../3rdparty/gmp
COMMON_LIB = -L../3rdparty/gmp/.libs -lgmp -lpthread
PARSER_LIB = ./parser/parser.a
PARSER_LIB_DEBUG = ./parser/parser_debug.a

all: pupp
	@cp pupp ~/bin/
	@echo all done.

debug: puppx
	@cp puppx ~/bin/
	@echo debug done.

puppx: puppx.o
	make -C parser debug
	g++ -o puppx puppx.o $(PARSER_LIB_DEBUG) $(COMMON_LIB)

pupp: pupp.o
	make -C parser
	g++ -o pupp pupp.o $(PARSER_LIB) $(COMMON_LIB)

pupp.o: pupp.cpp
	g++ -O3 -c $(INCLUDE) -o pupp.o pupp.cpp

puppx.o: pupp.cpp
	g++ -g -c $(INCLUDE) -o puppx.o pupp.cpp

clean:
	make -C parser clean
	rm -rf *.o pupp puppx


