/*
 * Copyright (C) 2017  Samantha Kacir
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
#include <algorithm>


typedef KnotVertex * (knotNot::*traceLettersFuncs)();

//get function pointers
traceLettersFuncs traceLetters[] = { &knotNot::getA, &knotNot::getB, &knotNot::getC, &knotNot::getD };
char letters[] = { 'a', 'b', 'c', 'd' };

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

  //doesn't count if if they share an endpoint
  if((a1x == b1x && a1y == b1y) ||
    (a2x == b1x && a2y == b1y) ||
    (a1x == b2x && a1y == b2y) ||
    (a2x == b2x && a2y == b2y)){
    return false;
  }

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

  while( k != last){
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

       knotNot newCrossing = knotNot(over1, over2, under1, under2);

       over1->insert(newCrossing);
       over2->insert(newCrossing);
       under1->insert(newCrossing);
       under2->insert(newCrossing);
     }
   }


   k = k->next;
 }
}

bool checkUpRightCusp(float slopeFirst, float slopeSecond){
  if (slopeFirst < slopeSecond) return true;
  return false;
}

bool checkDownRightCusp(float slopeFirst, float slopeSecond){
  if (slopeFirst > slopeSecond) return true;
  return false;
}

bool checkIfRightCusp(double x1, double x2, double x3){
  if(x1 <= x2 && x3 <= x2 && (x1 != x2 || x2 != x3)) return true;
  return false;
}

bool checkIfLeftCusp(double x1, double x2, double x3){
  if(x1 >= x2 && x3 >= x2 && (x1 != x2 || x2 != x3)) return true;
  return false;
}

bool checkIfCusp(double x1, double x2, double x3){
  if(checkIfRightCusp) return true;
  if(checkIfLeftCusp) return true;
  return false;
}

//cusp if x1 <= x2 >= x3

//r = totalDownCusps (prevY>nextY) - totalUpCusps (prevY<negY)

//|b + r| <= -1

//b + r is odd

int calculateR(KnotVertex * head){
  int count = 0;
  KnotVertex * cur = head;
  double * x1 = cur->prev->getX(), * x2 = cur->getX(), * x3 = cur->next->getX();
  float prevSlope = cur->prev->getSlopeToNext(),
   curSlope = cur->getSlopeToNext();

  if (checkIfRightCusp(*x1, *x2, *x3)){
    if (checkUpRightCusp(prevSlope, curSlope)) --count;
    else if (checkDownRightCusp(prevSlope, curSlope)) ++count;
  }
  else if (checkIfLeftCusp(*x1, *x2, *x3)){
    if (checkDownRightCusp(prevSlope, curSlope)) --count;
    else if (checkUpRightCusp(prevSlope, curSlope)) ++count;
  }
  cur = cur->next;
  prevSlope = curSlope;

  while(cur != head){
    x1 = x2;
    x2 = x3;
    x3 = cur->next->getX();
    curSlope = cur->getSlopeToNext();
    if (checkIfRightCusp(*x1, *x2, *x3)){
      if (checkUpRightCusp(prevSlope, curSlope)) --count;
      else if (checkDownRightCusp(prevSlope, curSlope)) ++count;
    }
    else if (checkIfLeftCusp(*x1, *x2, *x3)){
      if (checkDownRightCusp(prevSlope, curSlope)) --count;
      else if (checkUpRightCusp(prevSlope, curSlope)) ++count;
    }
    cur = cur -> next;
    prevSlope = curSlope;
  }

  return (count/2);
}

int totalNumberOfCusps(KnotVertex * head){
  int count = 0;
  KnotVertex * cur = head;
  double * x1 = cur->prev->getX(), * x2 = cur->getX(), * x3 = cur->next->getX();

  if (checkIfCusp(*x1, *x2, *x3)){
    count ++;
  }
  cur = cur->next->next;

  while(cur != head){
    x1 = x2;
    x2 = x3;
    x3 = cur->getX();
    if (checkIfCusp(*x1, *x2, *x3)){
      count ++;
    }
    cur = cur -> next;
  }

  return count;
}

int sumSigns(KnotVertex * head, int numOcross){  
  KnotVertex * k = head;
  knotNot crossingList[numOcross] = {};
  int count = 0;

  head->getAllCrossings(crossingList, numOcross);

  for(int i=0; i<numOcross; ++i){
    count += crossingList[i].getSign();
  }

  return count;
}

int calculateB(KnotVertex * head, int numOcross){
  int s = sumSigns(head, numOcross),
    c = totalNumberOfCusps(head); //note that c will always be even
  int b = s - (c/2);

  #ifdef DEBUG
  std::cout << s << " " << c << " " << b << std::endl;
  #endif

  return b;
}

void checkSameLine(int (* notNumbers)[crossComps], char (* notLetters)[crossComps], knotNot * crossingList, int i, int j){
  KnotVertex * initVertex = (crossingList[i].*traceLetters[j])(),
    * finalVertex = (crossingList[i].*traceLetters[(j+2)%crossComps])();
  vector<knotNot>* vectorCrossings = initVertex->getC();
  int vertexNumOcross = vectorCrossings->size();

  if(vertexNumOcross > 1){

    knotNot orderedCrossings[vertexNumOcross] = {};

    for(int vertI = 0; vertI < vertexNumOcross; ++vertI){
      orderedCrossings[vertI] = vectorCrossings->at(vertI);
    }

    #ifdef DEBUG
    std::cout << "There are: " << vertexNumOcross << " crossings in vertex " << initVertex->ident << std::endl;
    #endif

    //run through ordered list and assign notation
    for(int vertI = 0; vertI < vertexNumOcross - 1; ++vertI){
      for(int vertJ = 0; vertJ < crossComps; ++vertJ){
        if((orderedCrossings[vertI].*traceLetters[vertJ])() == finalVertex &&
        (orderedCrossings[vertI].*traceLetters[(vertJ+2)%crossComps])() == initVertex &&  vertI != vertexNumOcross - 1){
          for(int vertM = 0; vertM < crossComps; ++vertM){
            if((orderedCrossings[vertI + 1].*traceLetters[vertM])() == initVertex &&
              (orderedCrossings[vertI + 1].*traceLetters[(vertM+2)%crossComps])() == finalVertex){
              notNumbers[orderedCrossings[vertI].getLabel() - 1][vertJ] = orderedCrossings[vertI+1].getLabel();
              notLetters[orderedCrossings[vertI].getLabel() - 1][vertJ] = letters[vertM];

              notNumbers[orderedCrossings[vertI+1].getLabel() - 1][vertM] = orderedCrossings[vertI].getLabel();
              notLetters[orderedCrossings[vertI+1].getLabel() - 1][vertM] = letters[vertJ];

              #ifdef DEBUG
              std::cout << "same line elif: " << orderedCrossings[vertI].getLabel() - 1 << " is " << letters[vertJ] << " to " 
              << letters[vertM] << " with " << orderedCrossings[vertI+1].getLabel() - 1 << std::endl;
              #endif
            }
          }
        }
      }
    }
  }
}

void checkIfV(int (* notNumbers)[crossComps], char (* notLetters)[crossComps], knotNot * crossingList, int i, int j){
  KnotVertex * initVertex = (crossingList[i].*traceLetters[j])(),
    * finalVertex = (crossingList[i].*traceLetters[(j+2)%crossComps])();
  vector<knotNot>* vectorCrossings = initVertex->getC();
  int vertexNumOcross = vectorCrossings->size();
  bool foundCrossing = false;

  if(vertexNumOcross > 1){
    int vertI = 0;
    while(vertI < vertexNumOcross && !foundCrossing){
      knotNot crossingToCheck = vectorCrossings->at(vertI);
      //check if v
      for(int vertJ = 0; vertJ < crossComps; ++vertJ){
        if((crossingToCheck.*traceLetters[vertJ])() == initVertex &&
          (crossingToCheck.*traceLetters[(vertJ+2)%crossComps])() != finalVertex){
          //is v
          foundCrossing = true;

          notNumbers[i][j] = crossingToCheck.getLabel();
          notLetters[i][j] = letters[vertJ];

          notNumbers[crossingToCheck.getLabel() - 1][vertJ] = crossingList[i].getLabel();
          notLetters[crossingToCheck.getLabel() - 1][vertJ] = letters[j];

          #ifdef DEBUG
          std::cout << "v: " << i << " is " << letters[j] << " to "
           << letters[vertJ] << " with " << crossingToCheck.getLabel() - 1 << std::endl;
          #endif
        }
      }
      ++vertI;
    }
  }
}


bool generateNotation(KnotVertex * head, int numOcross, std::string tempFileName, bool br,std::string fileSuffix){
  if (numOcross == 0){
    if(br){
    ofstream storeBR;
    storeBR.open(("storeBR" + fileSuffix + "-NoCrossing" + ".txt").c_str(), std::ios_base::app);
    storeBR << calculateB(head, numOcross) << " " << calculateR(head) << std::endl;
    storeBR.close();
    }
    return true; 
  }

  if (br){
    ofstream storeBR;
    storeBR.open(("storeBR" + fileSuffix + ".txt").c_str(), std::ios_base::app);
    storeBR << calculateB(head, numOcross) << " " << calculateR(head) << std::endl;
    storeBR.close();
  }


  KnotVertex * k = head;
  knotNot crossingList[numOcross] = {};

  char notLetters[numOcross][4] = {};
  int notNumbers[numOcross][4] = {};

  head->getAllCrossings(crossingList, numOcross);

  #ifdef KNOTDETAILS
  std::cout << "CrossingList with " << numOcross << " crossings: " << std::endl;

    for(int i=0; i<numOcross; ++i){
      crossingList[i].printNot();
    }
  #endif

  //change so it uses vertices instead of crossings to minimize repeats
  for (int i = 0; i < numOcross; ++i){
    for (int j = 0; j < crossComps; ++j){
      if (!notLetters[i][j]) checkSameLine(notNumbers, notLetters, crossingList, i, j);
    }
  }
  //change so it uses vertices instead of crossings to minimize repeats
  for (int i = 0; i < numOcross; ++i){
    for (int j = 0; j < crossComps; ++j){
      if(!notLetters[i][j]) checkIfV(notNumbers, notLetters, crossingList, i, j);
    }
  }

  for (int i = 0; i < numOcross; ++i){
    int nextI, prevI;

    (i == numOcross - 1)?(nextI = 0):(nextI = i+1);
    (i == 0)?(prevI = numOcross-1):(prevI = i-1);

    int numsToCheck[] = { nextI, nextI, prevI, prevI },
      indicesONext[] = { 0, 1 };

    if (crossingList[i].getB()->next == crossingList[i].getD()){
      #ifdef DEBUG
      std::cout << "b goes prev" << std::endl;
      #endif
      numsToCheck[1] = prevI;
      numsToCheck[3] = nextI;
      indicesONext[1] = 3;
    }

  //have array of crossings from above. Starting with first, follow [a/b/c/d]->next->next->... until reach crossing; record label and a/b/c/d
  
    for (int j = 0; j < crossComps; ++j){
      #ifdef DEBUG
      std::cout << "Setting: " << letters[j] << i+1 << std::endl;
      #endif
      int checkIndex = (j+2)%crossComps;

      KnotVertex * initial = (crossingList[i].*traceLetters[j])(),
        * check = initial;
      while(!notNumbers[i][j]){
        if(j == 0 || j == indicesONext[1]){
          #ifdef DEBUG
          std::cout << "next" << std::endl;
          #endif
          for (int m = 0; m < numOcross; ++m){
            for (int l = 0; l < crossComps; ++l){
              if((l!=j || m!=i) && (crossingList[m].*traceLetters[l])() == check && !notNumbers[m][l]){
                notLetters[i][j] = letters[l];
                notNumbers[i][j] = crossingList[m].getLabel();

                notLetters[m][l] = letters[j];
                notNumbers[m][l] = crossingList[i].getLabel();
                goto foundNextCrossing;
              }
            }
          }
          check = check->next;
        }

        else{
          #ifdef DEBUG
          std::cout << "prev" << std::endl;
          #endif
          for (int m = numOcross-1; m > -1; --m){
            for (int l = 0; l < crossComps; ++l){
              if((l!=j || m!=i) && (crossingList[m].*traceLetters[l])() == check &&
               /*(m != i+1 || i == numOcross-2) &&*/ !notNumbers[m][l]){
                notLetters[i][j] = letters[l];
                notNumbers[i][j] = crossingList[m].getLabel();

                notLetters[m][l] = letters[j];
                notNumbers[m][l] = crossingList[i].getLabel();
                goto foundNextCrossing;
              }
            }
          }
          check = check->prev;
        }
        if(check == initial){
          std::cout << "No solution found for " << notLetters[i][j] << notNumbers[i][j] << std::endl;
          return false;
          break;
        }
        continue;

        foundNextCrossing:
        #ifdef DEBUG
        std::cout << "Different lines: " << notLetters[i][j] << notNumbers[i][j] << std::endl;
        #endif
        break;
      }
    }
  }

  ofstream tempOutputFile;
  k = head;

  tempOutputFile.open(tempFileName.c_str());

  #ifdef KNOTDETAILS
  std::cout << std::endl << "Generated Notation: " << std::endl;
  #endif

  for (int i = 0; i < numOcross; ++i){
    char sign = '+';
    if (crossingList[i].getSign()==-1) sign = '-';
    tempOutputFile << i+1 << sign;
    for (int j = 0; j < crossComps; ++j){
      //write to file
      tempOutputFile << notNumbers[i][j] << notLetters[i][j];
      //print to console
      #ifdef KNOTDETAILS
      std::cout << notLetters[i][j] << notNumbers[i][j] << " ";
      #endif
    }
    #ifdef KNOTDETAILS
    std::cout << std::endl;
    #endif
  }

  tempOutputFile.close();
  return true;
}

void generateKnot(KnotVertex* k, int n, ofstream &outputFile, bool br, std::string fileSuffix) {
  double xvals[n], yvals[n];

  for (int i=0; i<n; ++i){
    double tempx = ((double) rand() / (RAND_MAX)),
    tempy = ((double) rand() / (RAND_MAX));
    while(!k->validPoint(&tempx, &tempy)){
      tempx = ((double) rand() / (RAND_MAX));
      tempy = ((double) rand() / (RAND_MAX));
    }
    xvals[i] = tempx;
    yvals[i] = tempy;

    k->add(new KnotVertex(xvals + i, yvals + i));
  }
  returnCrossingIfCrossing(k, k);

  int numberOfCrossings = k->setCrossingVals();


  #ifdef KNOTDETAILS
  k->printAll();
  #endif


  #ifdef DEBUG
  std::cout << "number of crossings: " << numberOfCrossings << std::endl;
  #endif

  std::string generatedFileName = "tempGeneratedFile.txt";

  //failsafe:
  if (!generateNotation(k, numberOfCrossings, generatedFileName, br, fileSuffix)){
    free(k);
    KnotVertex * k = new KnotVertex();
    generateKnot(k, n, outputFile, br, fileSuffix);
  }

  #ifdef KNOTDETAILS
  std::cout << "B is: " << calculateB(k, numberOfCrossings) << std::endl;
  std::cout << "R is: " << calculateR(k) << std::endl;
  #endif

  ifstream tempOutputFile;

  tempOutputFile.open(generatedFileName.c_str());

  outputFile << tempOutputFile.rdbuf();

  tempOutputFile.close();
  remove(generatedFileName.c_str());
}
