
#include "knot.h"
#include <iostream>
#include <stdio.h>

Crossing::Crossing(double* overFirstX, double* overFirstY, double* overSecondX, double* overSecondY, double* underFirstX, double* underFirstY, double* underSecondX, double* underSecondY){
//nextUnder should be the understrand whose x value is between the two overs -- calc center point (avg(x), avg(y)); calc angle to each over; calc 1 under angle -> if between 2 over, = next; else prev
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
