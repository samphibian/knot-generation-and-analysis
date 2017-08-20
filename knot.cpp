/*
 * Copyright (C) 2017  Samantha Kacir
 *
 * The purpose of this assignment is to create a Circular List class for holding knots and crossings
 * and the functions needed to work with it.
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

using namespace std;


typedef KnotVertex * (knotNot::*traceLettersFuncs)();

//get function pointers
traceLettersFuncs lettersToTrace[] = { &knotNot::getA, &knotNot::getB, &knotNot::getC, &knotNot::getD };

KnotVertex::KnotVertex(){
  ident = 1;
  this->x = NULL;
  this->y = NULL;
  c = new vector<knotNot>();
  this->slopeToNext = 0;
  this->prev = this;
  this->next = this;
}

KnotVertex::KnotVertex (double * x, double * y) {
  ident = 1;
  this->x = x;
  this->y = y;
  c = new vector<knotNot>();
  this->slopeToNext = 0;
  this->prev = this;
  this->next = this;
}

KnotVertex::~KnotVertex(){
}

void KnotVertex::add(KnotVertex* v){
  if(this->x == NULL){
    this->x = v->getX();
    this->y = v->getY();
  }
  else{
    KnotVertex* temp = this->prev;
    // while(temp->next != this) {
    //   std::cout<<this << " : " << temp->next<<std::endl;
    //   temp = temp->next;
    // }
    
    returnCrossingIfCrossing(this, v);
    
    v->ident = temp->ident + 1;
    temp->next = v;
    v->prev = temp;
    v->next = this;
    this->prev = v;
    
    float curSlopeToNext = (float)(*(temp->next->y)-*(temp->y))/(*(temp->next->x)-*(temp->x));
    temp->slopeToNext = curSlopeToNext;
    
  #ifdef DEBUG
    std::cout << "temp x: " << *(temp->getX()) << ", temp y: " << *(temp->getY()) << ", tempnext x: " << *(temp->next->getX()) << ", tempnext y: " << *(temp->next->getY()) << " - slope: " << curSlopeToNext << std::endl;
  #endif
    
    float vSlopeToNext =  (float)(*(v->next->y)-*(v->y))/(*(v->next->x)-*(v->x));
    v->slopeToNext = vSlopeToNext;
    
  #ifdef DEBUG
    std::cout << "v x: " << *(v->getX()) << ", v y: " << *(v->getY()) << ", vnext x: " << *(v->next->getX()) << ", vnext y: " << *(v->next->getY()) << " - slope: "  << vSlopeToNext << std::endl;
  #endif
  }
}

void KnotVertex::insert(knotNot crossing){
  int loc = 0; //location of crossing
  #ifdef DEBUG
  std::cout << "Knot " << ident << ": ";
  #endif

  KnotVertex * check = new KnotVertex();

  for(int i=0; i < crossComps; ++i){
    if (this == (crossing.*lettersToTrace[i])()){
      check = (crossing.*lettersToTrace[(i+2)%crossComps])();
    }   
  }

  Point intersectionOfCrossing = crossing.getIntersection();
  //find proper location
  for(int i = 0; i<this->c->size(); ++i){
    bool checkAgainstI = true;
    for(int j=0; j < crossComps; ++j){
      if (this == (this->c->at(i).*lettersToTrace[j])() &&
      check != (this->c->at(i).*lettersToTrace[(j+2)%crossComps])()){
        checkAgainstI = false;
      }   
    }

    if(*(check->getX()) - *(this->getX()) < 0 && checkAgainstI){
      //compare x vals
      if(*(intersectionOfCrossing.getX()) > *(this->c->at(i).getIntersection().getX())){
        #ifdef DEBUG
        std::cout << "if" << std::endl;
        #endif
        goto foundloc;
      }
    }
    else if(*(check->getY())-*(this->getY()) < 0 && checkAgainstI){
      //compare y vals
      if(*(intersectionOfCrossing.getY()) > *(this->c->at(i).getIntersection().getY())){
        #ifdef DEBUG
        std::cout << "elif" << std::endl;
        #endif
        goto foundloc;
      }
    }
    else if (checkAgainstI){
      //compare x vals
      if(*(intersectionOfCrossing.getX()) < *(this-> c->at(i).getIntersection().getX())){
        #ifdef DEBUG
        std::cout << "else" << std::endl;
        #endif
        goto foundloc;
      }
    }
    ++loc;
  }
  #ifdef DEBUG
  std::cout << "n/a" << std::endl;
  #endif

  foundloc:
  std::vector<knotNot>::iterator it;
  it = this->c->begin();
  this->c->insert(it + loc, crossing);
}

double* KnotVertex::getX() {
  return this->x;
}

double* KnotVertex::getY() {
  return this->y;
}

bool KnotVertex::checkCrossing(){
  if (this->c->empty()){
    return 0;
  }
  return 1;
}

float KnotVertex::getSlopeToNext(){
  return this->slopeToNext;
}

knotNot* KnotVertex::getFirstCrossing() {
  if(this->c->empty()){
    std::cerr << "ERROR: vertex does not have a crossing" << std::endl;
    return 0;
  }
  
  return this->c->data();
}

vector<knotNot>* KnotVertex::getC(){
  return this->c;
}

void KnotVertex::remove(){
  if(next == this && prev==this){
    prev = NULL;
    next = NULL;
    x = NULL;
    y = NULL;
    c = NULL;
  }
  else{
    #ifdef DEBUG
    std::cout << "removing last point" << std::endl;
    #endif
    
    KnotVertex* n = this->prev;
    this->prev = n->prev;
    n->prev->next = this;
    n->prev = NULL;
    n->next = NULL;
    n->x = NULL;
    n->y = NULL;
    if(n->checkCrossing()){
      for(int m = 0; m < n->getC()->size(); ++m){
        knotNot crossing = n->getC()->at(m);
        for(int i=0; i<crossComps; ++i){
          if((crossing.*lettersToTrace[i])() != n){
            KnotVertex * match = (crossing.*lettersToTrace[i])();
            bool found = false;
            int j = -1, maxCount = match->getC()->size();
            
            #ifdef DEBUG
            std::cout<<maxCount<<": ";
            #endif

            while(!found && j < maxCount){
              ++j;
              found = true;
              for(int k=0; k<crossComps; ++k){
                if((match->getC()->at(j).*lettersToTrace[k])() != (crossing.*lettersToTrace[k])()) found = false;
              }
            }
            if (found) match->getC()->erase(match->getC()->begin() + j);
            
            #ifdef DEBUG
            std::cout<<match->getC()->size()<<std::endl;
            #endif

          }
        }
      }
    }
    n->c = NULL;

    this->slopeToNext = (float)(*(this->next->y)-*(this->y))/(*(this->next->x)-*(this->x));
  }
}

int KnotVertex::setCrossingVals(){
  KnotVertex * k = this;
  int lab = 0;
  while(k->next != this){
    if(k->checkCrossing()){
      #ifdef DEBUG
      std::cout << "crossings in k: " << k->getC()->size() << std::endl;
      #endif

      for(int i=0; i < k->getC()->size(); ++i){
        if(k->getC()->at(i).getLabel() < 0){
          ++lab;

          KnotVertex * a = k->getC()->at(i).getA(), //fromA will be same
            * b = k->getC()->at(i).getB(), //check fromB to see if same a and d            
            * c = k->getC()->at(i).getC(), //check fromC to see if same a and d
            * d = k->getC()->at(i).getD(); //if not, check fromD

          int numFound = 1;
          k->getC()->at(i).setLabel(lab);

          if(a->checkCrossing() && a != k){
            for(int j=0; j<a->getC()->size(); ++j){
              if(a->getC()->at(j).getB() == b && a->getC()->at(j).getC() == c && a->getC()->at(j).getD() == d){
                a->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting a\n\tLabel: " << a->getC()->at(j).getLabel() <<std::endl;
                #endif

                numFound++;
              }
            }
          }

          if(b->checkCrossing() && b != k && numFound < 4){
            for(int j=0; j<b->getC()->size(); ++j){
              if(b->getC()->at(j).getA() == a && b->getC()->at(j).getC() == c && b->getC()->at(j).getD() == d){
                b->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting b\n\tLabel: " << b->getC()->at(j).getLabel() <<std::endl;
                #endif

                numFound++;
              }
            }
          }

          if(c->checkCrossing() && c != k && numFound < 4){
            for(int j=0; j<c->getC()->size(); ++j){
              if(c->getC()->at(j).getB() == b && c->getC()->at(j).getA() == a && c->getC()->at(j).getD() == d){
                c->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting c\n\tLabel: " << c->getC()->at(j).getLabel() <<std::endl;
                #endif

                numFound++;
              }
            }
          }

          if(d->checkCrossing() && d != k && numFound < 4){
            for(int j=0; j<d->getC()->size(); ++j){
              if(d->getC()->at(j).getB() == b && d->getC()->at(j).getC() == c && d->getC()->at(j).getA() == a){
                d->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting d\n\tLabel: " << d->getC()->at(j).getLabel() <<std::endl;
                #endif

                numFound++;
              }
            }
          }
        }
      }
    }

    k = k->next;
  }

  if(k->checkCrossing()){
    #ifdef DEBUG
    std::cout << "crossings in k: " << k->getC()->size() << std::endl;
    #endif

    for(int i=0; i < k->getC()->size(); ++i){
      if(k->getC()->at(i).getLabel() < 0){
        ++lab;

        KnotVertex * a = k->getC()->at(i).getA(), //fromA will be same
          * b = k->getC()->at(i).getB(), //check fromB to see if same a and d            
          * c = k->getC()->at(i).getC(), //check fromC to see if same a and d
          * d = k->getC()->at(i).getD(); //if not, check fromD

        int numFound = 1;
        k->getC()->at(i).setLabel(lab);

        if(a->checkCrossing() && a != k){
          for(int j=0; j<a->getC()->size(); ++j){
            if(a->getC()->at(j).getB() == b && a->getC()->at(j).getC() == c && a->getC()->at(j).getD() == d){
              a->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting a\n\tLabel: " << a->getC()->at(j).getLabel() <<std::endl;checkCrossing
              #endif

              numFound++;
            }
          }
        }

        if(b->checkCrossing() && b != k && numFound < 4){
          for(int j=0; j<b->getC()->size(); ++j){
            if(b->getC()->at(j).getA() == a && b->getC()->at(j).getC() == c && b->getC()->at(j).getD() == d){
              b->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting b\n\tLabel: " << b->getC()->at(j).getLabel() <<std::endl;
              #endif

              numFound++;
            }
          }
        }

        if(c->checkCrossing() && c != k && numFound < 4){
          for(int j=0; j<c->getC()->size(); ++j){
            if(c->getC()->at(j).getB() == b && c->getC()->at(j).getA() == a && c->getC()->at(j).getD() == d){
              c->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting c\n\tLabel: " << c->getC()->at(j).getLabel() <<std::endl;
              #endif

              numFound++;
            }
          }
        }

        if(d->checkCrossing() && d != k && numFound < 4){
          for(int j=0; j<d->getC()->size(); ++j){
            if(d->getC()->at(j).getB() == b && d->getC()->at(j).getC() == c && d->getC()->at(j).getA() == a){
              d->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting d\n\tLabel: " << d->getC()->at(j).getLabel() <<std::endl;
              #endif

              numFound++;
            }
          }
        }
      }
    }
  }

  return lab;
}

void KnotVertex::printAll(){
  KnotVertex* current = this;
  
  bool d = 1;
  int index = 0;
  
  KnotVertex* first = current;
  
  this->print(0);
  
  while(current->next != first){
    current = current->next;
    ++index;
    this->print(index);
    std::cout << std::endl;
  }
}

void KnotVertex::print(int index){

  KnotVertex* current = this;
  
  int count = 0;
  
  while(count < index){
    current = current->next;    
    ++count;
  }

  
  cout << "The index " << current->ident << " vertex is located at (" << *(current->getX()) << ", " << *(current->getY()) << ") and the slope to the next vertex is: " 
  << current->slopeToNext << ". Does it have a crossing? " << (current->checkCrossing() ? ("Yes") : ("No")) << endl;
  if(current->checkCrossing()){
    for( int i=0; i<current->getC()->size(); ++i){
      cout << "\t";
      current->getC()->at(i).printNot();
    }
  }
}

bool KnotVertex::validPoint(double *xval, double *yval){
  KnotVertex *k = this;
  if(k->getX() == NULL){
    return true;
  }
  
  while(k->next != this){
    //test if point is already in the knot
    if(*(k->getX()) == *xval && *(k->getY()) == *(yval)){
      return false;
    }
    //test if point intersects with a line segment
    double b = (k->getSlopeToNext())*(*(k->getX())) - (*(k->getY()));

    if((((k->getSlopeToNext())*(*(xval))+b) == *(yval)) 
      && *xval > min(*(k->getX()), *(k->next->getX()))
      && *xval < max(*(k->getX()), *(k->next->getX()))){
      return false;
    }

    k = k->next;
  }
  //test if point is already the last point in the knot
  if(*(k->getX()) == *xval && *(k->getY()) == *(yval)){
    return false;
  }

  return true;
}

void KnotVertex::getAllCrossings(knotNot * crossingList, int numOcross){  
  KnotVertex * k = this;
  //check each crossing. note that the last doesn't need to be checked as all in that one shoud be duplicaties
  while(k->next != this){
    if (k->checkCrossing()){
      for(int i=0; i<k->getC()->size(); ++i){
        #ifdef DEBUG
        std::cout << "current crossing label is: " << crossingList[k->getC()->at(i).getLabel() - 1].getLabel() << std::endl;
        #endif
        //empty crossing generated with label -1, filled between 0 and number of crossings
        if(crossingList[k->getC()->at(i).getLabel() - 1].getLabel() < 0 || crossingList[k->getC()->at(i).getLabel() - 1].getLabel() > numOcross + 1){
          crossingList[k->getC()->at(i).getLabel() - 1] = k->getC()->at(i);

          #ifdef DEBUG
          std::cout << "label: " << k->getC()->at(i).getLabel() - 1 << std::endl;
          crossingList[k->getC()->at(i).getLabel() - 1].printNot();
          #endif
        }
      }
    }

    k = k->next;
  }
}

void testKnot(){
  int numKnot = 2;
  double d1 = 1.0, d2 = 2.0, dt = 1.5;
  KnotVertex * k = new KnotVertex(&d1, &d1);
  std::cout << "Testing: creating a KnotVertex" << std::endl;
  k->printAll();

  std::cout << "Testing: validPoint check - same point - " << ((k->validPoint(&d1, &d1) == 0) ? ("Invalid") : ("Valid")) << std::endl;
  
  std::cout << std::endl << "Testing: adding a KnotVertex" << std::endl;
  
  KnotVertex * k1 = new KnotVertex(&d2, &d2);
  k->add(k1);

  std::cout << "Testing: validPoint check - on line - " << ((k->validPoint(&dt, &dt) == 0) ? ("Invalid") : ("Valid")) << std::endl
  << "Testing: validPoint check - valid point - " << ((k->validPoint(&d1, &d2) == 0) ? ("Invalid") : ("Valid")) << std::endl;
  
  std::cout << std::endl << "Testing: getX() and getY()" << std::endl;
  KnotVertex * k2 = new KnotVertex(k->getX(), k1->getY());
  k->add(k2);

  std::cout << std::endl << "Testing: checkCrossing()" << std::endl;
  KnotVertex * k3 = new KnotVertex(k1->getX(), k->getY());
  k->add(k3);

  std::cout << std::endl << "Testing: entire knot structure" << std::endl;
  k->printAll();
}