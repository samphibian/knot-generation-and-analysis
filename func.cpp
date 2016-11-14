#include "knot.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>


bool validPoint(KnotVertex *head, double *xval, double *yval){
  KnotVertex *k = head;
  if(k->getX() == NULL){
    return true;
  }
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

void returnCrossingIfCrossing(KnotVertex *k, KnotVertex *n){
  KnotVertex* last = k->prev;
  double * xval = n->getX(),
    * yval = n->getY();
  
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
      KnotVertex *over1, *over2, *under1, *under2;
      if(k->getSlopeToNext() < slopeToNew){
	//new line under
	over1 = k;
	over2 = k->next;
	under1 = last;
	under2 = n;
      }
      else{	  
	//new line over
	under1 = k;
	under2 =k->next;
	over1 = last;
	over2 = n;
      }

      #ifdef DEBUG
      knotNot knt = knotNot(k, k->next, last, n);
      knotNot knt2 = knotNot(k, k->next, n, last); 
      knotNot knt3 = knotNot(k->next, k, last, n);
      knotNot knt4 = knotNot(k->next, k, n, last); 
      knt.printNot();
      knt2.printNot();
      knt3.printNot();
      knt4.printNot();
      #endif
	
      k->insert(knotNot(over1, over2, under1, under2));
      last->insert(knotNot(over1, over2, under1, under2));
    }
    
    k = k->next;
  }
}

vector<knotNot> generateNotation(KnotVertex * head){
  KnotVertex * k = head;
  static vector<knotNot> kNotation;
  int count = 0;

  if (k->checkCrossing()){
    for(int i=0; i<k->getC()->size(); ++i){
      ++count;
      kNotation.push_back(k->getC()->at(i));
      kNotation[i].setLabel(kNotation[i].getLabel()*count);
    }
  }
  

  while(k != head){
  }
}

void generateKnot(KnotVertex* k, int n) {
  double xvals[n], yvals[n];

  for (int i=0; i<n; ++i){
    double tempx = ((double) rand() / (RAND_MAX)),
      tempy = ((double) rand() / (RAND_MAX));
    while(!validPoint(k, &tempx, &tempy)){
      tempx = ((double) rand() / (RAND_MAX));
      tempy = ((double) rand() / (RAND_MAX));
    }
    xvals[i] = tempx;
    yvals[i] = tempy;

    k->add(new KnotVertex(xvals + i, yvals + i));    
  }

  //calculate
}
