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


bool generateNotation(KnotVertex * head, int numOcross, std::string tempFileName){
  KnotVertex * k = head;
  knotNot crossingList[numOcross] = {};

  char notLetters[numOcross][4] = {};
  int notNumbers[numOcross][4] = {};

  //check each crossing. note that the last doesn't need to be checked as all in that one shoud be duplicaties
  while(k->next != head){
    if (k->checkCrossing()){
      for(int i=0; i<k->getC()->size(); ++i){
        #ifdef DEBUG
        std::cout << "current crossing label is: " << crossingList[k->getC()->at(i).getLabel() - 1].getLabel() << std::endl;
        #endif
        //empty crossing generated with label -1, filled between 0 and number of crossings
        if(crossingList[k->getC()->at(i).getLabel() - 1].getLabel() < 0 || crossingList[k->getC()->at(i).getLabel() - 1].getLabel() > numOcross + 1){
          crossingList[k->getC()->at(i).getLabel() - 1] = k->getC()->at(i);

          #ifdef DEBUG
          std::cout << "label: " << k->getC()->at(i).getLabel() - 1 << std::endl;
          crossingList[k->getC()->at(i).getLabel() - 1].printNot();
          #endif
        }
      }
    }

    k = k->next;
  }


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

void generateKnot(KnotVertex* k, int n, ofstream &outputFile) {
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
  std::cout << "number of crossings: " << numOcross << std::endl;
  #endif

  // generateNotation(k, numberOfCrossings);

  std::string generatedFileName = "tempGeneratedFile.txt";

  //failsafe:
  if (!generateNotation(k, numberOfCrossings, generatedFileName)){
    free(k);
    KnotVertex * k = new KnotVertex();
    generateKnot(k, n, outputFile);
  }

  ifstream tempOutputFile;

  tempOutputFile.open(generatedFileName.c_str());

  outputFile << tempOutputFile.rdbuf();

  tempOutputFile.close();
}
