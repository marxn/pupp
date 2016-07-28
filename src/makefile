INCLUDE = -I../include -I../3rdparty/gmp
LIB = -L../3rdparty/gmp/.libs -lgmp
PARSER_SOURCE = closure.cpp  constval.cpp  container.cpp  expression.cpp  function.cpp  ifelse.cpp  loop.cpp  node.cpp  operation.cpp  statement.cpp  transaction.cpp  variable.cpp 
PROJECT_SOURCE = jsoncpp.cpp

all: pupp
	@cp pupp ~/bin/
	@echo all done.

pupp: parser.o scan.o main.o 
	g++ -g $(INCLUDE) -o pupp parser.o scan.o main.o $(PARSER_SOURCE) $(PROJECT_SOURCE) $(LIB)

parser.o: parser.y
	bison -d -o parser.cpp parser.y
	g++ -g -c $(INCLUDE) -o parser.o parser.cpp

scan.o: scan.l
	flex -o scan.cpp scan.l
	g++ -g -c $(INCLUDE) -o scan.o scan.cpp

main.o: main.cpp
	g++ -g -c $(INCLUDE) -o main.o main.cpp

clean:
	rm -rf *.o scan.cpp parser.cpp pupp parser.hpp 


