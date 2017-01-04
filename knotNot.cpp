//given a knot, should output notation. notation should have sign, crossing id, point of intersection, and the KnotVertex pointers for a, b, c, d

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

  double xavg = (*(over1->getX()) + *(over2->getX()) + *(under1->getX()) + *(under2->getX()))/4.0,
    yavg = (*(over1->getY()) + *(over2->getY()) + *(under1->getY()) + *(under2->getY()))/4.0;

  double thetaA = atan((*(over2->getY())-yavg)
			 /(*(over2->getX()) - xavg)),
    thetaC = atan((*(over1->getY())-yavg)
        /(*(over1->getX()) - xavg)),
    thetaUnder1 = atan((*(under1->getY())-yavg)
			 /(*(under1->getX()) - xavg));

  if((*(over2->getX()) - xavg) < 0){
    thetaA -= M_PI;
  }
  if((*(over1->getX()) - xavg) < 0){
    thetaC -= M_PI;
  }
  if((*(under1->getX()) - xavg) < 0){
    thetaUnder1 -= M_PI;
  }

  #ifdef DEBUG
  std::cout << "xavg: " << xavg << " yavg: " << yavg  << " thetaA: " << thetaA << " thetaC: " << thetaC << " thetaUnder1: " << thetaUnder1 << std::endl;
  #endif

  if(((*(over1->getY())>*(under1->getY())) != (*(over2->getY())>*(under1->getY()))) &&
      (*(under1->getX()) < 
        (((*(over2->getX()) - *(over1->getX()))*(*(under1->getY()) - *(over1->getY()))/
          (*(over2->getY()) - *(over1->getY()))) + *(over1->getX())))){
    //b counterclockwise
    fromB = under2;
    fromD = under1;
    sign = -1;
  }
  else{
    fromB = under1;
    fromD = under2;
    sign = 1;
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