
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "knot.h"

//a knot should be a linked list of nodes with an xval, a yval, (or create point class - probably better. Point: double x, double y) and if it has a crossing. There should be a list of crossings. Each crossing should have the index of the points of the line it crosses with and say if it's top or bottom.

int main(){
	srand(time(NULL));
  KnotVertex * knot = new KnotVertex();

  int n;
  
  #ifdef DEBUG
  testKnot();
  #endif

  std::cout << "How many points would you like to generate for the knot? ";
  std::cin >> n;
  std::cout << std::endl;
  generateKnot(knot, n);
}



/****
Sources:
http://stackoverflow.com/questions/9878965/c-rand-between-0-and-1
http://www.cplusplus.com/forum/beginner/49408/ (Disch's answer)
http://stackoverflow.com/questions/14176776/find-out-if-2-lines-intersect
*****/
