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
