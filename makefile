# Copyright (C) 2017  Samantha Kacir

# The Knot List makefile

# This file is part of knot-generation-and-analysis.

# knot-generation-and-analysis is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# knot-generation-and-analysis is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with knot-generation-and-analysis.  If not, see <http://www.gnu.org/licenses/>.
 
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
