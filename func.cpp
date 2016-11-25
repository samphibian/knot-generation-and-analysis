#include "knot.h"
#include <string>
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

bool checkIntersect(Point a1, Point a2, Point b1, Point b2){
  /*
  ((A2-A0)*(B1-B0) - (B2-B0)*(A1-A0)) * ((A3-A0)*(B1-B0) - (B3-B0)*(A1-A0)) < 0
  &&
  ((A0-A2)*(B3-B2) - (B0-B2)*(A3-A2)) * ((A1-A2)*(B3-B2) - (B1-B2)*(A3-A2)) < 0
  */

  double a1x = *(a1.getX()),
    a1y = *(a1.getY()),
    a2x = *(a2.getX()),
    a2y = *(a2.getY()),
    b1x = *(b1.getX()),
    b1y = *(b1.getY()),
    b2x = *(b2.getX()),
    b2y = *(b2.getY());

  double firstTest = ((b1x - a1x)*(a2y - a1y) - (b1y - a1y)*(a2x - a1x)) * ((b2x - a1x)*(a2y - a1y) - (b2y - a1y)*(a2x - a1x)),
    secondTest = ((a1x - b1x)*(b2y - b1y) - (a1y - b1y)*(b2x - b1x)) * ((a2x - b1x)*(b2y - b1y) - (a2y - b1y)*(b2x - b1x));

  #ifdef DEBUG
  std::cout << "first test: " << signbit(firstTest)<< std::endl;

  std::cout << "second test: " <<  signbit(secondTest) << std:: endl;
  #endif

  return (signbit(firstTest) && signbit(secondTest));
}

void returnCrossingIfCrossing(KnotVertex *k, KnotVertex *n){
  KnotVertex* last = k->prev;
  double * xval = n->getX(),
  * yval = n->getY();

  float slopeToNew = (float)(*(yval)-*(last->getY()))/(*(xval)-*(last->getX()));

  while( k->next != last){
    if(k!=n){
      Point a1 = Point(k->getX(), k->getY()),
      a2 = Point(k->next->getX(), k->next->getY()),
      b1 = Point(last->getX(), last->getY()),
      b2 = Point(xval, yval);

      if (checkIntersect(a1, a2, b1, b2)){
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
      kNotation[i].setLabel(kNotation[i].getSign()*count);
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
    //k->printAll(); 
  }
  returnCrossingIfCrossing(k, k);

  k->printAll();
}
