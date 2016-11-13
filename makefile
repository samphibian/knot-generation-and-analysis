
 # Samantha Kacir
 # The Knot List makefile
 
all: run

run: knots.o func.o knot.o crossing.o point.o
	g++ knots.o func.o knot.o crossing.o point.o -o run

func.o: func.cpp
	g++ -c func.cpp -o func.o

point.o: point.cpp
	g++ -c point.cpp -o point.o

crossing.o: crossing.cpp
	g++ -c crossing.cpp -o crossing.o

knot.o: knot.cpp
	g++ -c knot.cpp -o knot.o

knots.o: knots.cpp
	g++ -c knots.cpp -o knots.o

clean:
	rm run
	rm -f *.o
