INCLUDE = -I../../include -I../../3rdparty/gmp
PARSER_SOURCE = closure.cpp  constval.cpp  container.cpp  expression.cpp  function.cpp  ifelse.cpp  loop.cpp  node.cpp  operation.cpp  statement.cpp  transaction.cpp  variable.cpp 

all: parser.a
	@echo parser built.

debug: parser_debug.a
	@echo parser_debug build.

parser_debug.a: parser.y scan.l $(PARSER_SOURCE)
	bison -d -o parser.cpp parser.y
	flex -o scan.cpp scan.l
	g++ -g -c $(INCLUDE) parser.cpp scan.cpp $(PARSER_SOURCE)
	ar -r parser_debug.a *.o

parser.a: parser.y scan.l $(PARSER_SOURCE)
	bison -d -o parser.cpp parser.y
	flex -o scan.cpp scan.l
	g++ -O3 -c $(INCLUDE) parser.cpp scan.cpp $(PARSER_SOURCE)
	ar -r parser.a *.o

clean:
	rm -rf *.a *.o scan.cpp parser.cpp parser.hpp 


