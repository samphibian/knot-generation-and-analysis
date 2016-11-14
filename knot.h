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

class Crossing{
  int identity;
  double *olsfpx; //over line segment first point x
  double *olsfpy;
  double *olsspx;
  double *olsspy;
  double *ulsfpx; //under line segment first point x
  double *ulsfpy;
  double *ulsspx;
  double *ulsspy;
 public:
  Crossing(double* overFirstX, double* overFirstY, double* overSecondX, double* overSecondY, double* underFirstX, double* underFirstY, double* underSecondX, double* underSecondY);
  ~Crossing();
  void updateIdent(int n);
  int getIdent();
};
  
class KnotVertex{
  double *x;
  double *y;
  float slopeToNext;
  vector<Crossing> *c;
  //KnotVertex *prev;
  //KnotVertex *next;
 public:
  KnotVertex *prev;
  KnotVertex *next;
  KnotVertex();
  KnotVertex(double * x, double * y);
  ~KnotVertex();
  void add(KnotVertex* v);
  void insert(Crossing crossing);
  double* getX();
  double* getY();
  bool checkCrossing();
  float getSlopeToNext();
  Crossing* getFirstCrossing();
  vector<Crossing>* getC();
  void remove();
  void printAll(); // prints all data
  void print(int index); // prints the data index points away from the given one
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

vector<knotNot> generateNotation(KnotVertex * head);
bool validPoint(KnotVertex *head, double *xval, double *yval);
void returnCrossingIfCrossing(KnotVertex *k, KnotVertex *n);
void testKnot();
void generateKnot(KnotVertex* k, int n);

#endif
