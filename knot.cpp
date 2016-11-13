/*
 * Samantha Kacir
 * The purpose of this assignment is to create a Circular List class for holding knots and crossings
 * and the functions needed to work with it.
*/

/*
Looked at:
http://stackoverflow.com/questions/14176776/find-out-if-2-lines-intersect
 */
//enum ListType{INTEGER, FLOAT, CHAR, STRING, GENERIC_LIST};


#include "knot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>

using namespace std;

Point::Point(double* xval, double* yval){
  x = xval;
  y = yval;
}
double * Point::getX(){
  return this->x;
}
double * Point::getY(){
  return this->y;
}

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

KnotVertex::KnotVertex(){
  //static float sTn;
  this->x = NULL;
  this->y = NULL;
  c = new vector<Crossing>();
  //this->slopeToNext = &sTn;
  this->slopeToNext = 0;
  prev = this;
  next = this;
}
KnotVertex::KnotVertex (double * x, double * y) {
  this->x = x;
  this->y = y;
  c = new vector<Crossing>();
  //this->slopeToNext = NULL;
  this->slopeToNext = 0;
  prev = this;
  next = this;
}

KnotVertex::~KnotVertex(){
}

void KnotVertex::add(KnotVertex* v){
  KnotVertex* temp = this;
  while(temp->next != this) {
    temp = temp->next;
  }
  
  returnCrossingIfCrossing(this, v->getX(), v->getY());
  
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
  temp->slopeToNext = vSlopeToNext;
  
  #ifdef DEBUG
  std::cout << "v x: " << *(v->getX()) << ", v y: " << *(v->getY()) << ", vnext x: " << *(v->next->getX()) << ", vnext y: " << *(v->next->getY()) << " - slope: "  << vSlopeToNext << std::endl;
  #endif
}

void KnotVertex::insert(Crossing crossing){
  this->c->push_back(crossing);
}

double* KnotVertex::getX() {
  return this->x;
}

double* KnotVertex::getY() {
  return this->y;
}

bool KnotVertex::checkCrossing(KnotVertex* v){
  if (!this->c->empty()){
    return true;
  }
  else{
    return false;
  }
}

bool validPoint(KnotVertex *head, double *xval, double *yval){
  KnotVertex *k = head;
  while(k->next != head){
    //test if point is already in the knot
    if(*(k->getX()) == *xval && *(k->getY()) == *(yval)){
      return false;
    }
    //test if point intersects with a line segment
    //ADD CHECK FOR IF BETWEEN ENDPOINTS!!!
    double b = (k->getSlopeToNext())*(*(k->getX())) - (*(k->getY()));
    if((((k->getSlopeToNext())*(*(xval))+b) == *(yval))){
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

void returnCrossingIfCrossing(KnotVertex *k, double *xval, double *yval){
  KnotVertex* last = k->prev;
  float slopeToNew = (float)(*(yval)-*(last->getY()))/(*(xval)-*(last->getX()));
  
  while( k->next != last->next){
    double firstTest = ((*(k->getX()) - *(last->getX()))*(*yval - *(last->getY()))
	  - (*(k->getY())-*(last->getY()))*(*xval - *(last->getX()))
	 ) * ((*(k->next->getX()) - *(last->getX()))*(*yval - *(last->getY()))
	      - (*(k->next->getY()) - *(last->getY()))*(*xval - *(last->getX()))),
      secondTest = ((*(last->getX()) - *(k->getX()))*(*(k->next->getY()) - *(k->getY()))
	  - (*yval-*(k->getY()))*(*(k->next->getX()) - *(k->getX()))
         ) * (
              (*(last->getX()) - *(k->getX()))*(*(k->next->getY()) - *(k->getY()))
              -	(*(last->getY()) - *(k->getY()))*(*(k->next->getX()) - *(k->getX())));

    #ifdef DEBUG
    std::cout << "first test: " << signbit(firstTest)<< std::endl;

    std::cout << "second test: " <<  signbit(secondTest) << std:: endl;
    #endif
    
    if ((signbit(firstTest)==1) && (signbit(secondTest) == 1)){
      Point *over1, *over2, *under1, *under2;
      if(k->getSlopeToNext() < slopeToNew){
	//new line under
	over1 = new Point(k->getX(), k->getY());
	over2 = new Point(k->next->getX(), k->next->getY());
	under1 = new Point(last->getX(), last->getY());
	under2 = new Point(xval, yval);
      }
      else{	  
	//new line over
	under1 = new Point(k->getX(), k->getY());
	under2 = new Point(k->next->getX(), k->next->getY());
	over1 = new Point(last->getX(), last->getY());
	over2 = new Point(xval, yval);
      }
      k->insert(Crossing(over1->getX(), over1->getY(),
			 over2->getX(), over2->getY(),
			 under1->getX(), under1->getY(),
			 under2->getX(), under2->getY()));
      last->insert(Crossing(over1->getX(), over1->getY(),
			 over2->getX(), over2->getY(),
			 under1->getX(), under1->getY(),
			 under2->getX(), under2->getY()));
      k->getCrossing()->updateIdent(1);
      last->getCrossing()->updateIdent(1);
    }
    
    k = k->next;
  }
}

float KnotVertex::getSlopeToNext(){
  return this->slopeToNext;
}

Crossing* KnotVertex::getCrossing() {
  if(this->c->empty()){
    return 0;
  }
  
  return this->c->data();
}

int KnotVertex::getCrossingIdent() {
  if(this->c->empty()){
    return 0;
  }
  
  return this->c->data()->getIdent();
}

void KnotVertex::remove(){
  if(next == this && prev==this){
    prev = NULL;
    next = NULL;
    x = NULL;
    y = NULL;
    c = NULL;
    //slopeToNext = NULL;
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
    //n->slopeToNext = NULL;

    this->slopeToNext = (float)(*(this->next->y)-*(this->y))/(*(this->next->x)-*(this->x));
  }
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
  }
}

void KnotVertex::print(int index){

  KnotVertex* current = this;
  
  int count = 0;
  
  while(count < index){
    current = current->next;    
    ++count;
  }

  
  cout << "The index " << index << " vertex is located at (" << *(current->x) << ", " << *(current->y) << ") and the slope to the next vertex is: " << current->slopeToNext << ". Does it have a crossing? " << ((current->getCrossingIdent() == 0) ? ("No") : ("Yes")) << endl;
}


void testKnot(){
  int numKnot = 2;
  double d1 = 1.0, d2 = 2.0, dt = 1.5;
  KnotVertex * k = new KnotVertex(&d1, &d1);
  std::cout << "Testing: creating a KnotVertex" << std::endl;
  k->printAll();

  std::cout << "Testing: validPoint check - same point - " << ((validPoint(k, &d1, &d1) == 0) ? ("Invalid") : ("Valid")) << std::endl;
  
  std::cout << std::endl << "Testing: adding a KnotVertex" << std::endl;
  
  KnotVertex * k1 = new KnotVertex(&d2, &d2);
  k->add(k1);

  std::cout << "Testing: validPoint check - on line - " << ((validPoint(k, &dt, &dt) == 0) ? ("Invalid") : ("Valid")) << std::endl
	    << "Testing: validPoint check - valid point - " << ((validPoint(k, &d1, &d2) == 0) ? ("Invalid") : ("Valid")) << std::endl;
  
  std::cout << std::endl << "Testing: getX() and getY()" << std::endl;
  KnotVertex * k2 = new KnotVertex(k->getX(), k1->getY());
  k->add(k2);

  std::cout << std::endl << "Testing: isCrossing()" << std::endl;
  KnotVertex * k3 = new KnotVertex(k1->getX(), k->getY());
  k->add(k3);

  std::cout << std::endl << "Testing: entire knot structure" << std::endl;
  k->printAll();
}
