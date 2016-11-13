
#include <stdlib.h>
#include <math.h>
#include "knot.h"

//a knot should be a linked list of nodes with an xval, a yval, (or create point class - probably better. Point: double x, double y) and if it has a crossing. There should be a list of crossings. Each crossing should have the index of the points of the line it crosses with and say if it's top or bottom.

void generateKnot(int n) {
  double xvals[n], yvals[n];

  for (int i=0; i<n; ++i){
    xvals[i] = ((double) rand() / (RAND_MAX));
    yvals[i] = ((double) rand() / (RAND_MAX));

    //check if it crosses a previous one
    if (i > 1) {
      for (int j=0; j<i; ++j) {
	//if cross, check if top/bottom and add crossing to both
      }
    }
  }

  //check if line between final and first cross any others

  //calculate
}
int main(){
  testKnot();
}



/****
Sources:
http://stackoverflow.com/questions/9878965/c-rand-between-0-and-1
http://www.cplusplus.com/forum/beginner/49408/ (Disch's answer)
*****/
