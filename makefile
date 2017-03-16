
 # Samantha Kacir
 # The Knot List makefile
 
all: run

run: knots.o func.o knot.o  point.o knotNot.o
	g++ knots.o func.o knot.o point.o knotNot.o -o run

func.o: func.cpp
	g++ -c func.cpp -o func.o

knotNot.o: knotNot.cpp
	g++ -c knotNot.cpp -o knotNot.o

point.o: point.cpp
	g++ -c point.cpp -o point.o

knot.o: knot.cpp
	g++ -c knot.cpp -o knot.o

knots.o: knots.cpp
	g++ -c knots.cpp -o knots.o

clean:
	rm -f *.o
	rm run
