/*
 * Samantha Kacir
 * The purpose of this is to define the classes necessary to store knots
 * and the functions needed to work with them.
*/

#ifndef KNOT_H
#define KNOT_H

#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

class Point;
class knotNot;
class KnotVertex;

//enum ListType{INTEGER, FLOAT, CHAR, STRING, GENERIC_LIST};

class Point{
  double *x;
  double *y;
 public:
  Point(double * xval, double * yval);
  ~Point();
  double * getX();
  double * getY();
};
  
class knotNot{
  int label;
  KnotVertex * fromA;
  KnotVertex * fromB;
  KnotVertex * fromC;
  KnotVertex * fromD;
 public:
  knotNot();
  knotNot(KnotVertex * over1, KnotVertex * over2, KnotVertex * under1, KnotVertex * under2);
  ~knotNot();
  void setLabel(int n);
  void setA(KnotVertex *a);
  void setB(KnotVertex *b);
  void setC(KnotVertex *c);
  void setD(KnotVertex *d);
  int getLabel();
  KnotVertex * getA();
  KnotVertex * getB();
  KnotVertex * getC();
  KnotVertex * getD();
  void printNot();
};

class KnotVertex{
  double *x;
  double *y;
  float slopeToNext;
  vector<knotNot> *c;
  //KnotVertex *prev;
  //KnotVertex *next;
 public:
  KnotVertex *prev;
  KnotVertex *next;
  KnotVertex();
  KnotVertex(double * x, double * y);
  ~KnotVertex();
  void add(KnotVertex* v);
  void insert(knotNot crossing);
  double* getX();
  double* getY();
  bool checkCrossing();
  float getSlopeToNext();
  knotNot* getFirstCrossing();
  vector<knotNot>* getC();
  void remove();
  void printAll(); // prints all data
  void print(int index); // prints the data index points away from the given one
};


vector<knotNot> generateNotation(KnotVertex * head);
bool validPoint(KnotVertex *head, double *xval, double *yval);
void returnCrossingIfCrossing(KnotVertex *k, KnotVertex *n);
void testKnot();
void generateKnot(KnotVertex* k, int n);

#endif
