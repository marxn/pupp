all: puppy
	@echo all done.

puppy: puppy.o scan.o main.o 
	g++ -g -o puppy puppy.o scan.o main.o

puppy.o: puppy.y
	bison -d -o puppy.cpp puppy.y
	g++ -g -c -I../include -o puppy.o puppy.cpp

scan.o: scan.l
	flex -o scan.cpp scan.l
	g++ -g -c -I../include -o scan.o scan.cpp

main.o: main.cpp
	g++ -g -c -I../include -o main.o main.cpp

clean:
	rm -rf *.o scan.cpp puppy.cpp puppy puppy.hpp 

