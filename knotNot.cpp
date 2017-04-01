/*
 * Copyright (C) 2017  Samantha Kacir
 *
 * given a knot, should output notation. notation should have sign, crossing id, point of intersection, and the KnotVertex pointers for a, b, c, d
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
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <math.h>

knotNot::knotNot(){
  label = -1;
  sign = 0;
  intersection = Point();
  fromA = NULL;
  fromB = NULL;
  fromC = NULL;
  fromD = NULL;
}

knotNot::knotNot(KnotVertex * over1, KnotVertex * over2, KnotVertex * under1, KnotVertex * under2){
  label = -1;
  sign = 0;
  fromA = over2;
  fromC = over1;
  intersection = Point();

// Use the sign of the determinant of vectors (O1O2,O1U1), where O1, O2 are the Over1/2 and U1 is Under1

  if(((*(over2->getX()) - *(over1->getX())) * (*(under1->getY()) - *(over1->getY())))
    <((*(over2->getY()) - *(over1->getY())) * (*(under1->getX()) - *(over1->getX())))){
    //b clockwise
    fromB = under1;
    fromD = under2;
    sign = 1;
  }
  else{
    fromB = under2;
    fromD = under1;
    sign = -1;
 }

  //p+tr=q+us
  //t = (q − p) × s / (r × s)
  //u = (q − p) × r / (r × s)
  //r = p2-p1
  //s = q2-q1

  double rx = *(this->fromC->getX())-*(this->fromA->getX()),
    ry = *(this->fromC->getY())-*(this->fromA->getY()),
    sx = *(under2->getX())-*(under1->getX()), 
    sy = *(under2->getY())-*(under1->getY());

  Point p = Point(this->fromA->getX(), this->fromA->getY()),
    r = Point(&rx, &ry),
    q = Point(under1->getX(), under1->getY()),
    s = Point(&sx, &sy);

  double qminuspx = *(q.getX())-*(p.getX()),
    qminuspy = *(q.getY())-*(p.getY());

  Point qminusp = Point(&qminuspx, &qminuspy);

  //vx wy − vy wx 
  double tnum = ((*(qminusp.getX()))*(*(s.getY()))) - ((*(qminusp.getY()))*(*(s.getX()))), //t numerator
    tden = ((*(r.getX()))*(*(s.getY()))) - ((*(r.getY()))*(*(s.getX()))),
    t = tnum/tden;

  //p + tr
    *(intersection.getX()) = *(p.getX()) + t*(*(r.getX()));
    *(intersection.getY()) = *(p.getY()) + t*(*(r.getY()));
}

knotNot::~knotNot(){
}


Point knotNot::getIntersection(){
  return this->intersection;
}

void knotNot::setSign(int n){
  this->sign = n;
}

void knotNot::setLabel(int n){
  #ifdef DEBUG
  std::cout << "Set label to: " << n << std::endl;
  #endif

  this->label = n;
}

int knotNot::getSign(){
  return this->sign;
}

int knotNot::getLabel(){
  return this->label;
}

void knotNot::setA(KnotVertex *a){
  this->fromA = a;
}

KnotVertex * knotNot::getA(){
  return this->fromA;
}

void knotNot::setB(KnotVertex *b){
  this->fromB = b;
}

KnotVertex * knotNot::getB(){
  return this->fromB;
}

void knotNot::setC(KnotVertex *c){
  this->fromC = c;
}

KnotVertex * knotNot::getC(){
  return this->fromC;
}

void knotNot::setD(KnotVertex *d){
  this->fromD = d;
}

KnotVertex * knotNot::getD(){
  return this->fromD;
}

void knotNot::printNot(){
  std::cout << "Crossing " << this->getLabel() <<
    " is at the point ";
  
  this->intersection.print();
  
  std::cout << std::endl << "\t\t" <<
    "a: (" << *(this->getA()->getX()) << ", " <<
    *(this->getA()->getY()) << "), b: (" <<
    *(this->getB()->getX()) << ", " <<
    *(this->getB()->getY()) << "), c: (" <<
    *(this->getC()->getX()) << ", " <<
    *(this->getC()->getY()) << "), d: (" <<
    *(this->getD()->getX()) << ", " <<
    *(this->getD()->getY()) << ")" << std::endl;
}