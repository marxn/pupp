all: pupp
	@cp pupp ~/bin/
	@echo all done.

pupp: pupp.o scan.o main.o 
	g++ -g -o pupp pupp.o scan.o main.o -lgmp

pupp.o: pupp.y
	bison -d -o pupp.cpp pupp.y
	g++ -g -c -I../include -o pupp.o pupp.cpp

scan.o: scan.l
	flex -o scan.cpp scan.l
	g++ -g -c -I../include -o scan.o scan.cpp

main.o: main.cpp
	g++ -g -c -I../include -o main.o main.cpp

clean:
	rm -rf *.o scan.cpp pupp.cpp pupp pupp.hpp 


