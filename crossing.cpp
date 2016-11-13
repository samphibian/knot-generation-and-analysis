
#include "knot.h"
#include <iostream>
#include <stdio.h>

Crossing::Crossing(double* overFirstX, double* overFirstY, double* overSecondX, double* overSecondY, double* underFirstX, double* underFirstY, double* underSecondX, double* underSecondY){
  identity = 0;
  olsfpx = overFirstX,
    olsfpy = overFirstY,
    olsspx = overSecondX,
    olsspy = overSecondY,
    ulsfpx = underFirstX,
    ulsfpy = underFirstY,
    ulsspx = underSecondX,
    ulsspy = underSecondY;
}
Crossing::~Crossing(){
}

void Crossing::updateIdent(int n){
  this->identity = n;
}

int Crossing::getIdent(){
  #ifdef DEBUG
  std::cout << this->identity << std::endl;
  #endif
  return this->identity;
}
