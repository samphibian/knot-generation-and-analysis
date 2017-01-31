/*
 * Samantha Kacir
 * The purpose of this assignment is to create a Circular List class for holding knots and crossings
 * and the functions needed to work with it.
*/

#include "knot.h"

 using namespace std;

 KnotVertex::KnotVertex(){
  ident = 1;
  this->x = NULL;
  this->y = NULL;
  c = new vector<knotNot>();
  this->slopeToNext = 0;
  prev = this;
  next = this;
}

KnotVertex::KnotVertex (double * x, double * y) {
  ident = 1;
  this->x = x;
  this->y = y;
  c = new vector<knotNot>();
  this->slopeToNext = 0;
  prev = this;
  next = this;
}

KnotVertex::~KnotVertex(){
}

void KnotVertex::add(KnotVertex* v){
  if(this->x == NULL){
    this->x = v->getX();
    this->y = v->getY();
  }
  else{
    KnotVertex* temp = this;
    while(temp->next != this) {
      temp = temp->next;
    }
    
    returnCrossingIfCrossing(this, v);
    
    v->ident = temp->ident + 1;
    temp->next = v;
    v->prev = temp;
    v->next = this;
    this -> prev = v;
    
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

  Point intersectionOfCrossing = crossing.getIntersection();
  //find proper location
  for(int i = 0; i<this->c->size(); ++i){
    if(*(this->next->getX()) - *(this->getX()) < 0){
      //compare x vals
      if(*(intersectionOfCrossing.getX()) > *(this->c->at(i).getIntersection().getX())){
        goto foundloc;
      }
    }
    else if(*(this->next->getY())-*(this->getY()) < 0){
      //compare y vals
      if(*(intersectionOfCrossing.getY()) > *(this->c->at(i).getIntersection().getY())){
        goto foundloc;
      }
    }
    else{
      //compare x vals
      if(*(intersectionOfCrossing.getX()) < *(this-> c->at(i).getIntersection().getX())){
        goto foundloc;
      }
    }
    ++loc;
  }

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
    KnotVertex* n = next;
    this->next = n->next;
    n->next->prev = this;
    n->prev = NULL;
    n->next = NULL;
    n->x = NULL;
    n->y = NULL;
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

          bool foundPair = false;
          k->getC()->at(i).setLabel(lab);

          if(a->checkCrossing() && a != k){
            for(int j=0; j<a->getC()->size(); ++j){
              if(a->getC()->at(j).getB() == b && a->getC()->at(j).getC() == c && a->getC()->at(j).getD() == d){
                a->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting a\n\tLabel: " << a->getC()->at(j).getLabel() <<std::endl;checkCrossing
                #endif

                foundPair = true;
              }
            }
          }

          if(b->checkCrossing() && b != k && !foundPair){
            for(int j=0; j<b->getC()->size(); ++j){
              if(b->getC()->at(j).getA() == a && b->getC()->at(j).getC() == c && b->getC()->at(j).getD() == d){
                b->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting b\n\tLabel: " << b->getC()->at(j).getLabel() <<std::endl;
                #endif

                foundPair = true;
              }
            }
          }

          if(c->checkCrossing() && c != k && !foundPair){
            for(int j=0; j<c->getC()->size(); ++j){
              if(c->getC()->at(j).getB() == b && c->getC()->at(j).getA() == a && c->getC()->at(j).getD() == d){
                c->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting c\n\tLabel: " << c->getC()->at(j).getLabel() <<std::endl;
                #endif

                foundPair = true;
              }
            }
          }

          if(d->checkCrossing() && d != k && !foundPair){
            for(int j=0; j<d->getC()->size(); ++j){
              if(d->getC()->at(j).getB() == b && d->getC()->at(j).getC() == c && d->getC()->at(j).getA() == a){
                d->getC()->at(j).setLabel(lab);

                #ifdef DEBUG
                std::cout << "Setting d\n\tLabel: " << d->getC()->at(j).getLabel() <<std::endl;
                #endif

                foundPair = true;
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

        bool foundPair = false;
        k->getC()->at(i).setLabel(lab);

        if(a->checkCrossing() && a != k){
          for(int j=0; j<a->getC()->size(); ++j){
            if(a->getC()->at(j).getB() == b && a->getC()->at(j).getC() == c && a->getC()->at(j).getD() == d){
              a->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting a\n\tLabel: " << a->getC()->at(j).getLabel() <<std::endl;checkCrossing
              #endif

              foundPair = true;
            }
          }
        }

        if(b->checkCrossing() && b != k && !foundPair){
          for(int j=0; j<b->getC()->size(); ++j){
            if(b->getC()->at(j).getA() == a && b->getC()->at(j).getC() == c && b->getC()->at(j).getD() == d){
              b->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting b\n\tLabel: " << b->getC()->at(j).getLabel() <<std::endl;
              #endif

              foundPair = true;
            }
          }
        }

        if(c->checkCrossing() && c != k && !foundPair){
          for(int j=0; j<c->getC()->size(); ++j){
            if(c->getC()->at(j).getB() == b && c->getC()->at(j).getA() == a && c->getC()->at(j).getD() == d){
              c->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting c\n\tLabel: " << c->getC()->at(j).getLabel() <<std::endl;
              #endif

              foundPair = true;
            }
          }
        }

        if(d->checkCrossing() && d != k && !foundPair){
          for(int j=0; j<d->getC()->size(); ++j){
            if(d->getC()->at(j).getB() == b && d->getC()->at(j).getC() == c && d->getC()->at(j).getA() == a){
              d->getC()->at(j).setLabel(lab);

              #ifdef DEBUG
              std::cout << "Setting d\n\tLabel: " << d->getC()->at(j).getLabel() <<std::endl;
              #endif

              foundPair = true;
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