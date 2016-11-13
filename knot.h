/*
 * Samantha Kacir
 * The purpose of this is to create a Circular List class to hold knots
 * and the functions needed to work with it.
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
  bool checkCrossing(KnotVertex* v);
  float getSlopeToNext();
  Crossing* getCrossing();
  int getCrossingIdent();
  void remove();
  void printAll(); // prints all data
  void print(int index); // prints the data index points away from the given one
};

bool validPoint(KnotVertex *head, double *xval, double *yval);
void returnCrossingIfCrossing(KnotVertex *k, double *xval, double *yval);
void testKnot();

#endif


