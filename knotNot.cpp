//given a knot, should output notation. notation should be {crossing id: a, b, c, d} - vector?

#include "knot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>

knotNot::knotNot(){
  label = 0;
  fromA = NULL;
  fromB = NULL;
  fromC = NULL;
  fromD = NULL;
}

knotNot::~knotNot(){
}

void knotNot::setLabel(int n){
  this->label = n;
}

void knotNot::setA(KnotVertex *a){
  this->fromA = a;
}

void knotNot::setB(KnotVertex *b){
  this->fromB = b;
}

void knotNot::setC(KnotVertex *c){
  this->fromC = c;
}

void knotNot::setD(KnotVertex *d){
  this->fromD = d;
}
