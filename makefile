
 # Samantha Kacir
 # The Knot List makefile
 
all: run

run: knots.o knot.o
	g++ knots.o knot.o -o run

knot.o: knot.cpp
	g++ -c knot.cpp -o knot.o

knots.o: knots.cpp
	g++ -c knots.cpp -o knots.o

clean:
	rm run
	rm -f *.o
