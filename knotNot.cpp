//given a knot, should output notation. notation should be {crossing id: a, b, c, d} - vector?

#include "knot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <math.h>

knotNot::knotNot(){
  label = 0;
  fromA = NULL;
  fromB = NULL;
  fromC = NULL;
  fromD = NULL;
}

knotNot::knotNot(KnotVertex * over1, KnotVertex * over2, KnotVertex * under1, KnotVertex * under2){
  //NEED TO COME UP WITH +/-
  label = 0;
  fromA = over2;
  fromC = over1;

  double xavg = (*(over1->getX()) + *(over2->getX()) + *(under1->getX()) + *(under2->getX()))/4.0,
    yavg = (*(over1->getY()) + *(over2->getY()) + *(under1->getY()) + *(under2->getY()))/4.0;

  double thetaA = atan((*(over2->getY())-yavg)
			 /(*(over2->getX()) - xavg)),
    thetaUnder1 = atan((*(under1->getY())-yavg)
			 /(*(under1->getX()) - xavg));

  if((*(over2->getX()) - xavg) < 0){
    thetaA += M_PI;
  }
  if((*(under1->getX())-xavg)<0){
    thetaUnder1 += M_PI;
  }

  #ifdef DEBUG
  std::cout << "xavg: " << xavg << " yavg: " << yavg  << " " << thetaA << " " << thetaUnder1 << std::endl;
  #endif

  //bcd clockwise
  if(thetaUnder1 > (thetaA - M_PI)  && thetaUnder1 > thetaA){
      fromB = under1;
      fromD = under2;
    }
  else{
    fromB = under2;
    fromD = under1;
  }
}

knotNot::~knotNot(){
}

void knotNot::setLabel(int n){
  this->label = n;
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
  std::cout << "Crossing: " << this->getLabel() <<
    ", a: (" << *(this->getA()->getX()) << ", " <<
    *(this->getA()->getY()) << "), b: (" <<
    *(this->getB()->getX()) << ", " <<
    *(this->getB()->getY()) << "), c: (" <<
    *(this->getC()->getX()) << ", " <<
    *(this->getC()->getY()) << "), d: (" <<
    *(this->getD()->getX()) << ", " <<
    *(this->getD()->getY()) << ")" << std::endl;
}

