/*
 * Copyright (C) 2017  Samantha Kacir
 * 
 * This file is part of knot-generation-and-analysis.
 * 
 * knot-generation-and-analysis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * knot-generation-and-analysis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with knot-generation-and-analysis.  If not, see <http://www.gnu.org/licenses/>.
 */

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
