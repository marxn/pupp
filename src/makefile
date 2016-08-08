INCLUDE = -I../include -I../3rdparty/gmp
COMMON_LIB = -L../3rdparty/gmp/.libs -lgmp
PARSER_LIB = ./parser/parser.a

all: pupp
	@cp pupp ~/bin/
	@echo all done.

pupp: pupp.o
	make -C parser
	g++ -o pupp pupp.o $(PARSER_LIB) $(COMMON_LIB)

pupp.o: pupp.cpp
	g++ -g -c $(INCLUDE) -o pupp.o pupp.cpp

clean:
	make -C parser clean
	rm -rf *.o pupp 


