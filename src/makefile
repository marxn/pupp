INCLUDE = -I../include -I../3rdparty/gmp
LIB = -L../3rdparty/gmp/.libs -lgmp
PROJECT_SOURCE = jsoncpp.cpp

all: pupp
	@cp pupp ~/bin/
	@echo all done.

pupp: pupp.o scan.o main.o 
	g++ -g $(INCLUDE) -o pupp pupp.o scan.o main.o $(PROJECT_SOURCE) $(LIB)

pupp.o: pupp.y
	bison -d -o pupp.cpp pupp.y
	g++ -g -c $(INCLUDE) -o pupp.o pupp.cpp

scan.o: scan.l
	flex -o scan.cpp scan.l
	g++ -g -c $(INCLUDE) -o scan.o scan.cpp

main.o: main.cpp
	g++ -g -c $(INCLUDE) -o main.o main.cpp

clean:
	rm -rf *.o scan.cpp pupp.cpp pupp pupp.hpp 


