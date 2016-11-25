
#include "knot.h"

Point::Point(){
	x = new double();
	y = new double();
}
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
void Point::print(){
	std::cout << "(" << *(this->x) << ", " << *(this->y) << ")";
}
