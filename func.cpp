#include "knot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>


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
