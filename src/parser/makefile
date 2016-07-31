INCLUDE = -I../../include -I../../3rdparty/gmp
PARSER_SOURCE = closure.cpp  constval.cpp  container.cpp  expression.cpp  function.cpp  ifelse.cpp  loop.cpp  node.cpp  operation.cpp  statement.cpp  transaction.cpp  variable.cpp 

all: parser.a
	@echo parser built.

parser.a: parser.y scan.l $(PARSER_SOURCE)
	bison -d -o parser.cpp parser.y
	flex -o scan.cpp scan.l
	g++ -g -c $(INCLUDE) parser.cpp scan.cpp $(PARSER_SOURCE)
	ar -r parser.a *.o

clean:
	rm -rf parser.a *.o scan.cpp parser.cpp parser.hpp 


