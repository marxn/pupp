INCLUDE = -I../include -I../3rdparty/gmp
COMMON_LIB = -L../3rdparty/gmp/.libs -lgmp
PARSER_LIB = ./parser/parser.a

all: pupp
	@cp pupp ~/bin/
	@echo all done.

pupp: main.o
	make -C parser
	g++ -o pupp main.o $(PARSER_LIB) $(COMMON_LIB)

main.o: main.cpp
	g++ -g -c $(INCLUDE) -o main.o main.cpp

clean:
	make -C parser clean
	rm -rf *.o pupp 


