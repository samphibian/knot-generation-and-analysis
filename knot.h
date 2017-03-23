/*
 * Samantha Kacir
 * The purpose of this is to define the classes necessary to store knots
 * and the functions needed to work with them.
*/

#ifndef KNOT_H
#define KNOT_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <cmath>

#define crossComps 4 //crossing components: a, b, c, d

using namespace std;

class Point;
class knotNot;
class KnotVertex;

//enum ListType{INTEGER, FLOAT, CHAR, STRING, GENERIC_LIST};

class Point{
  double *x;
  double *y;
 public:
  Point();
  Point(double * xval, double * yval);
  ~Point();
  double * getX();
  double * getY();
  void print();
};
  
class knotNot{
  int label;
  int sign;
  Point intersection;
  KnotVertex * fromA;
  KnotVertex * fromB;
  KnotVertex * fromC;
  KnotVertex * fromD;
 public:
  knotNot();
  knotNot(KnotVertex * over1, KnotVertex * over2, KnotVertex * under1, KnotVertex * under2);
  ~knotNot();
  void setSign(int n);
  void setLabel(int n);
  void setA(KnotVertex *a);
  void setB(KnotVertex *b);
  void setC(KnotVertex *c);
  void setD(KnotVertex *d);
  int getSign();
  int getLabel();
  Point getIntersection();
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
 public:
  int ident;
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
  int setCrossingVals(); //loop through knot, assign numbers to crossings, return highest
  void printAll(); // prints all data
  void print(int index); // prints the data index points away from the given one
  bool validPoint(double *xval, double *yval); // check if new point valid
};


// vector<knotNot> generateNotation(KnotVertex * head);
void returnCrossingIfCrossing(KnotVertex *k, KnotVertex *n);
void testKnot();
void generateKnot(KnotVertex* k, int n, ofstream & outputFile);

#endif
