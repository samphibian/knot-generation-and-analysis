
 # Samantha Kacir
 # The Knot List makefile
 
all: run

run: homflyExe.o knots.o func.o knot.o  point.o knotNot.o parse.o
	g++ homflyExe.o knots.o func.o knot.o point.o knotNot.o parse.o -o run

parse.o: parseHomflyOutput.cpp
	g++ -c parseHomflyOutput.cpp -o parse.o

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

homflyExe.o: milletHomfly.c
	gcc -c milletHomfly.c -o homflyExe.o

clean:
	rm *.o
	rm *.txt
	rm *.out
	rm run
