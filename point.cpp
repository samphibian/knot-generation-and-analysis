
#include "knot.h"
#include <iostream>
#include <stdio.h>


Point::Point(double* xval, double* yval){
  x = xval;
  y = yval;
}
Point::~Point(){
	
}
double * Point::getX(){
  return this->x;
}
double * Point::getY(){
  return this->y;
}
