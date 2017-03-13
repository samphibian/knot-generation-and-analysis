#include "knot.h"
#include <algorithm>

#include <fstream>

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

void checkSameLine(int (* notNumbers)[4], char (* notLetters)[4], knotNot * crossingList, int i, int j){
  typedef KnotVertex * (knotNot::*traceLettersFuncs)();

  //get function pointers
  traceLettersFuncs traceLetters[] = { &knotNot::getA, &knotNot::getB, &knotNot::getC, &knotNot::getD };

  int crossComps = 4;
  KnotVertex * initVertex = (crossingList[i].*traceLetters[j])(),
    * finalVertex = (crossingList[i].*traceLetters[(j + crossComps - 1)%crossComps])();
  vector<knotNot>* vectorCrossings = initVertex->getC();
  int vertexNumOcross = vectorCrossings->size();

  if(vertexNumOcross > 1){
    char letters[] = { 'a', 'b', 'c', 'd' };

    knotNot orderedCrossings[vertexNumOcross] = {};

    for(int vertI = 0; vertI < vertexNumOcross; ++vertI){
      orderedCrossings[vertI]=vectorCrossings->at(vertI);
    }

    std::cout << "There are: " << vertexNumOcross << " crossings in vertex " << initVertex->ident << std::endl;

    for(int vertI = 0; vertI < vertexNumOcross; ++vertI){
      Point curInt = orderedCrossings[vertI].getIntersection();
      for(int vertJ = vertI + 1; vertJ < vertexNumOcross; ++vertJ){
        //find closest intersection between vectorCrossings[vertI] and end
        Point testInt = orderedCrossings[vertJ].getIntersection();
        //since they're all on the same line, only need to test x or y
        if(sqrt((curInt.getX() - initVertex->getX())^2 + (curInt.getY() - initVertex->getY())^2) > 
          sqrt((testInt.getX() - initVertex->getX())^2 + (testInt.getY() - initVertex->getY())^2)){
          #ifdef DEBUG
          vectorCrossings->at(vertJ).printNot();
          vectorCrossings->at(vertI).printNot();
          #endif
          orderedCrossings[vertI] = vectorCrossings->at(vertJ);
          orderedCrossings[vertJ] = vectorCrossings->at(vertI);
        }
      }
    }

    #ifdef DEBUG
    for(int k=0; k<vertexNumOcross; ++k){
      orderedCrossings[k].printNot();
    }
    #endif

    //run through ordered list and assign notation
    for(int vertI = 0; vertI < vertexNumOcross - 1; ++vertI){
      for(int vertJ = 0; vertJ < crossComps; ++vertJ){
        if((orderedCrossings[vertI].*traceLetters[vertJ])() == initVertex && vertI != 0){
          for(int vertM = 0; vertM < crossComps; ++vertM){
            if((orderedCrossings[vertI + 1].*traceLetters[vertM])() == finalVertex){
              notNumbers[orderedCrossings[vertI].getLabel() - 1][vertJ] = orderedCrossings[vertI+1].getLabel();
              notLetters[orderedCrossings[vertI].getLabel() - 1][vertJ] = letters[vertM];

              notNumbers[orderedCrossings[vertI+1].getLabel() - 1][vertM] = orderedCrossings[vertI].getLabel();
              notLetters[orderedCrossings[vertI+1].getLabel() - 1][vertM] = letters[vertJ];

              std::cout << "same line if: " << orderedCrossings[vertI].getLabel() - 1 << " is " << letters[vertJ] << " to " << letters[vertM]
             << " with " << orderedCrossings[vertI+1].getLabel() - 1 << std::endl;
            }
          }
        }
        else if((orderedCrossings[vertI].*traceLetters[vertJ])() == finalVertex && vertI != vertexNumOcross - 1){
          for(int vertM = 0; vertM < crossComps; ++vertM){
            if((orderedCrossings[vertI + 1].*traceLetters[vertM])() == initVertex){
              notNumbers[orderedCrossings[vertI].getLabel() - 1][vertJ] = orderedCrossings[vertI+1].getLabel();
              notLetters[orderedCrossings[vertI].getLabel() - 1][vertJ] = letters[vertM];

              notNumbers[orderedCrossings[vertI+1].getLabel() - 1][vertM] = orderedCrossings[vertI].getLabel();
              notLetters[orderedCrossings[vertI+1].getLabel() - 1][vertM] = letters[vertJ];

              std::cout << "same line elif: " << orderedCrossings[vertI].getLabel() - 1 << " is " << letters[vertJ] << " to " << letters[vertM]
             << " with " << orderedCrossings[vertI+1].getLabel() - 1 << std::endl;
            }
          }
        }
      }
    }
  }
}

bool generateNotation(KnotVertex * head, int numOcross){
  int crossComps = 4; //a, b, c, d
  KnotVertex * k = head;
  knotNot crossingList[numOcross] = {};

  char notLetters[numOcross][4] = {};
  int notNumbers[numOcross][4] = {};
  typedef KnotVertex * (knotNot::*traceLettersFuncs)();

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

  std::cout << "CrossingList with " << numOcross << " crossings: " << std::endl;
    for(int i=0; i<numOcross; ++i){
      crossingList[i].printNot();
    }

  //get function pointers
  traceLettersFuncs traceLetters[] = { &knotNot::getA, &knotNot::getB, &knotNot::getC, &knotNot::getD };
  char letters[] = { 'a', 'b', 'c', 'd' };

  //have array of crossings from above. Starting with first, follow [a/b/c/d]->next->next->... until reach crossing; record label and a/b/c/d
  for (int i = 0; i < numOcross; ++i){
    int nextI, prevI;

    (i == numOcross - 1)?(nextI = 0):(nextI = i+1);
    (i == 0)?(prevI = numOcross-1):(prevI = i-1);

    int numsToCheck[] = { nextI, nextI, prevI, prevI },
      indicesONext[] = { 0, 1 };

    if (crossingList[i].getB()->next == crossingList[i].getD()){
      std::cout << "b goes prev" << std::endl;
      numsToCheck[1] = prevI;
      numsToCheck[3] = nextI;
      indicesONext[1] = 3;
    }

    for (int j = 0; j < crossComps; ++j){
      checkSameLine(notNumbers, notLetters,crossingList, i, j);

      if(!notLetters[i][j]){
        std::cout << "Setting: " << letters[j] << i+1 << std::endl;
        int checkIndex = (j+2)%crossComps;

        // if( numOcross > 1 &&
        //  (crossingList[i].*traceLetters[j])() == (crossingList[numsToCheck[j]].*traceLetters[(j + crossComps - 1)%crossComps])() &&
        //  (crossingList[i].*traceLetters[checkIndex])() == (crossingList[numsToCheck[j]].*traceLetters[(checkIndex + crossComps - 1)%crossComps])() ){
        //   ///shared line ACDB, BDAC, CABD, DBCA
        //   std::cout << "Same 4- line " << std::endl;
        //   if( (checkIndex == 0 || checkIndex == indicesONext[1]) &&
        //    (crossingList[i].*traceLetters[checkIndex])()->next != (crossingList[i].*traceLetters[j])() ||
        //     ( checkIndex != 0 && checkIndex != indicesONext[1] && 
        //    (crossingList[i].*traceLetters[checkIndex])()->prev != (crossingList[i].*traceLetters[j])() )){
        //     notLetters[i][checkIndex] = letters[(j + crossComps - 1)%crossComps];
        //     notNumbers[i][checkIndex] = crossingList[numsToCheck[j]].getLabel();

        //     notLetters[numsToCheck[j]][(j + crossComps - 1)%crossComps] = letters[checkIndex];
        //     notNumbers[numsToCheck[j]][(j + crossComps - 1)%crossComps] = crossingList[i].getLabel();
        //     std::cout << "if: " << i << " is " << letters[checkIndex] << " to " << letters[(j + crossComps - 1)%crossComps]
        //      << " with " << numsToCheck[j] << std::endl;
        //   }
        //   // else if ( checkIndex != 0 && checkIndex != indicesONext[1] && 
        //   //  (crossingList[i].*traceLetters[checkIndex])()->prev != (crossingList[i].*traceLetters[j])() ){
        //   //   notLetters[i][checkIndex] = letters[(j + crossComps - 1)%crossComps];
        //   //   notNumbers[i][checkIndex] = crossingList[numsToCheck[j]].getLabel();

        //   //   notLetters[numsToCheck[j]][(j + crossComps - 1)%crossComps] = letters[checkIndex];
        //   //   notNumbers[numsToCheck[j]][(j + crossComps - 1)%crossComps] = crossingList[i].getLabel();
        //   //   std::cout << "else if: " << i << " is " << letters[checkIndex] << " to " << letters[(j + crossComps - 1)%crossComps]
        //   //    << " with " << numsToCheck[j] << std::endl;
        //   // }
        //   else{
        //     notLetters[i][j] = letters[(checkIndex + crossComps - 1)%crossComps];
        //     notNumbers[i][j] = crossingList[numsToCheck[j]].getLabel();

        //     notLetters[numsToCheck[j]][(checkIndex + crossComps - 1)%crossComps] = letters[j];
        //     notNumbers[numsToCheck[j]][(checkIndex + crossComps - 1)%crossComps] = crossingList[i].getLabel();
        //     std::cout << "else: " << i << " is " << letters[j] << " to " << letters[(checkIndex + crossComps - 1)%crossComps]
        //      << " with " << numsToCheck[j] << std::endl;
        //   }
        // }
        // else if( numOcross > 1 &&
        //  (crossingList[i].*traceLetters[j])() == (crossingList[numsToCheck[j]].*traceLetters[checkIndex])() &&
        //  (crossingList[i].*traceLetters[(j + crossComps - 1)%crossComps])() == (crossingList[numsToCheck[j]].*traceLetters[(checkIndex + crossComps - 1)%crossComps])() ){
        //   ///shared line ACDB, BDAC, CABD, DBCA
        //   std::cout << "Same 4+ line " << std::endl;
        //   if( ((j + crossComps - 1)%crossComps == 0 || (j + crossComps - 1)%crossComps == indicesONext[1]) &&
        //    (crossingList[i].*traceLetters[(j + crossComps - 1)%crossComps])()->next != (crossingList[i].*traceLetters[j])() ||
        //    ( (j + crossComps - 1)%crossComps != 0 && (j + crossComps - 1)%crossComps != indicesONext[1] && 
        //    (crossingList[i].*traceLetters[(j + crossComps - 1)%crossComps])()->prev != (crossingList[i].*traceLetters[j])() )){
        //     notLetters[i][(j + crossComps - 1)%crossComps] = letters[checkIndex];
        //     notNumbers[i][(j + crossComps - 1)%crossComps] = crossingList[numsToCheck[j]].getLabel();

        //     notLetters[numsToCheck[j]][checkIndex] = letters[(j + crossComps - 1)%crossComps];
        //     notNumbers[numsToCheck[j]][checkIndex] = crossingList[i].getLabel();
            // std::cout << "if: " << i << " is " << letters[(j + crossComps - 1)%crossComps] << " to " << letters[checkIndex]
            //  << " with " << numsToCheck[j] << std::endl;
        //   }
        //   // else if ( checkIndex != 0 && checkIndex != indicesONext[1] && 
        //   //  (crossingList[i].*traceLetters[checkIndex])()->prev != (crossingList[i].*traceLetters[j])() ){
        //   //   notLetters[i][checkIndex] = letters[(j + crossComps - 1)%crossComps];
        //   //   notNumbers[i][checkIndex] = crossingList[numsToCheck[j]].getLabel();

        //   //   notLetters[numsToCheck[j]][(j + crossComps - 1)%crossComps] = letters[checkIndex];
        //   //   notNumbers[numsToCheck[j]][(j + crossComps - 1)%crossComps] = crossingList[i].getLabel();
        //   //   std::cout << "else if: " << i << " is " << letters[checkIndex] << " to " << letters[(j + crossComps - 1)%crossComps]
        //   //    << " with " << numsToCheck[j] << std::endl;
        //   // }
        //   else{
        //     notLetters[i][j] = letters[(checkIndex + crossComps - 1)%crossComps];
        //     notNumbers[i][j] = crossingList[numsToCheck[j]].getLabel();

        //     notLetters[numsToCheck[j]][(checkIndex + crossComps - 1)%crossComps] = letters[j];
        //     notNumbers[numsToCheck[j]][(checkIndex + crossComps - 1)%crossComps] = crossingList[i].getLabel();
        //     std::cout << "else: " << i << " is " << letters[j] << " to " << letters[(checkIndex + crossComps - 1)%crossComps]
        //      << " with " << numsToCheck[j] << std::endl;
        //   }
        // }
        // //trace each
        // else if(i == 0  && numOcross > 2 &&
        //  (((crossingList[i].*traceLetters[j])() == (crossingList[numOcross - 1].*traceLetters[j])() &&
        //  (crossingList[i].*traceLetters[checkIndex])() == (crossingList[numOcross - 1].*traceLetters[checkIndex])()) ||
        //  ((crossingList[i].*traceLetters[j])() == (crossingList[numOcross - 1].*traceLetters[checkIndex])() &&
        //  (crossingList[i].*traceLetters[checkIndex])() == (crossingList[numOcross - 1].*traceLetters[j])())) &&
        //  i != numOcross - 1 &&
        //  !notNumbers[numOcross - 1][checkIndex] && !notNumbers[i][j]){
        //   //shared line
        //   std::cout << "Same line 0->numOcross-1 ";

        //   if((crossingList[i].*traceLetters[checkIndex])() == (crossingList[numOcross - 1].*traceLetters[j])()){
        //     if( ((j == 0 || j == indicesONext[1]) && numOcross - 1 < i) ||
        //      (j != 0 && j != indicesONext[1] && numOcross - 1 > i) ){
        //       notLetters[i][j] = letters[j];
        //       notNumbers[i][j] = crossingList[numOcross - 1].getLabel();

        //       notLetters[numOcross - 1][checkIndex] = letters[checkIndex];
        //       notNumbers[numOcross - 1][checkIndex] = crossingList[i].getLabel();
        //       std::cout << "j->cI: " << i << " is " << letters[j] << " to " << letters[j] << " with " << numOcross - 1 << std::endl;
        //     }
        //     else{
        //       notLetters[i][checkIndex] = letters[j];
        //       notNumbers[i][checkIndex] = crossingList[numOcross - 1].getLabel();

        //       notLetters[numOcross - 1][j] = letters[checkIndex];
        //       notNumbers[numOcross - 1][j] = crossingList[i].getLabel();
        //       std::cout << "cI->j: " << i << " is " << letters[checkIndex] << " to " << letters[j] << " with " << numOcross - 1 << std::endl;
        //     }
        //   }
        // }
        // else if((((crossingList[i].*traceLetters[j])() == (crossingList[nextI].*traceLetters[j])() &&
        //  (crossingList[i].*traceLetters[checkIndex])() == (crossingList[nextI].*traceLetters[checkIndex])()) ||
        //  ((crossingList[i].*traceLetters[j])() == (crossingList[nextI].*traceLetters[checkIndex])() &&
        //  (crossingList[i].*traceLetters[checkIndex])() == (crossingList[nextI].*traceLetters[j])())) &&
        //  i != nextI &&
        //  !notNumbers[nextI][checkIndex] && !notNumbers[i][j]){
        //   //shared line
        //   std::cout << "Same line ";

        //   if((crossingList[i].*traceLetters[checkIndex])() == (crossingList[nextI].*traceLetters[j])()){
        //     if ( (j != 0 && j != indicesONext[1]) &&
        //      (crossingList[i].*traceLetters[j])()->prev != (crossingList[i].*traceLetters[(j+1)%crossComps])() ){
        //       notLetters[i][checkIndex] = letters[checkIndex];
        //       notNumbers[i][checkIndex] = crossingList[nextI].getLabel();

        //       notLetters[nextI][checkIndex] = letters[checkIndex];
        //       notNumbers[nextI][checkIndex] = crossingList[i].getLabel();
        //       std::cout << "cI->j 1: " << i << " is " << letters[checkIndex] << " to " << letters[checkIndex]
        //        << " with " << nextI << std::endl;
        //     }
        //     else if (j == 0 || j == indicesONext[1] &&
        //      (crossingList[i].*traceLetters[j])()->next != (crossingList[i].*traceLetters[(j+1)%crossComps])() ){
        //       notLetters[i][j] = letters[j];
        //       notNumbers[i][j] = crossingList[nextI].getLabel();

        //       notLetters[nextI][j] = letters[j];
        //       notNumbers[nextI][j] = crossingList[i].getLabel();
        //       std::cout << "cI->j 2: " << i << " is " << letters[j] << " to " << letters[j]
        //        << " with " << nextI << std::endl;
        //     }
        //   }

        //   else if( (checkIndex != 0 && checkIndex != indicesONext[1]) &&
        //      (crossingList[i].*traceLetters[checkIndex])()->next != (crossingList[i].*traceLetters[(checkIndex+1)%crossComps])()){
        //     notLetters[i][j] = letters[checkIndex];
        //     notNumbers[i][j] = crossingList[nextI].getLabel();

        //     notLetters[nextI][checkIndex] = letters[j];
        //     notNumbers[nextI][checkIndex] = crossingList[i].getLabel();
        //     std::cout << "j->cI 1: " << i << " is " << letters[j] << " to " << letters[checkIndex] << " with " << nextI << std::endl;
        //   }
        //   else if( checkIndex == 0 || checkIndex == indicesONext[1] &&
        //      (crossingList[i].*traceLetters[checkIndex])()->prev != (crossingList[i].*traceLetters[(checkIndex+1)%crossComps])() ){
        //     notLetters[i][checkIndex] = letters[j];
        //     notNumbers[i][checkIndex] = crossingList[nextI].getLabel();

        //     notLetters[nextI][j] = letters[checkIndex];
        //     notNumbers[nextI][j] = crossingList[i].getLabel();
        //     std::cout << "j->cI 2: " << i << " is " << letters[checkIndex] << " to " << letters[j] << " with " << nextI << std::endl;
        //   }
        // }
        if ((crossingList[i].*traceLetters[j])() == (crossingList[numsToCheck[j]].*traceLetters[j])() &&
         i!= numsToCheck[j] &&
         !notNumbers[numsToCheck[j]][j]){
          //v
          notLetters[i][j] = letters[j];
          notNumbers[i][j] = crossingList[numsToCheck[j]].getLabel();

          notLetters[numsToCheck[j]][j] = letters[j];
          notNumbers[numsToCheck[j]][j] = crossingList[i].getLabel();
          std::cout << "V i->numsToCheck: " << i << " is " << letters[j] << " to " << letters[j] << " with " << numsToCheck[j] << std::endl;
        }
        else if (i == 0  && numOcross > 2 &&
          (crossingList[i].*traceLetters[j])() == (crossingList[numOcross - 1].*traceLetters[j])() &&
         !notNumbers[numOcross - 1][j]){
          //v
          notLetters[i][j] = letters[j];
          notNumbers[i][j] = crossingList[numOcross - 1].getLabel();

          notLetters[numOcross - 1][j] = letters[j];
          notNumbers[numOcross - 1][j] = crossingList[i].getLabel();
          std::cout << "V i->numOcross-1: " << i << " is " << letters[j] << " to " << letters[j] << " with " << numOcross - 1 << std::endl;
        }
        // else{
          KnotVertex * initial = (crossingList[i].*traceLetters[j])(),
            * check = initial;
          while(!notNumbers[i][j]){
            if(j == 0 || j == indicesONext[1]){
              std::cout << "next" << std::endl;
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
              std::cout << "prev" << std::endl;
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
            std::cout << "Different lines: " << notLetters[i][j] << notNumbers[i][j] << std::endl;
            break;
          // }
        }
      }
    }
  }

  ofstream outputFile;
  k = head;

  outputFile.open("generatedNotation.txt");

  for (int i = 0; i < numOcross; ++i){
    char sign = '+';
    if (crossingList[i].getSign()==-1) sign = '-';
    outputFile << i+1 << sign;
    for (int j = 0; j < crossComps; ++j){
      //write to file
      outputFile << notNumbers[i][j] << notLetters[i][j];
      //print to console
      std::cout << notLetters[i][j] << notNumbers[i][j] << " ";
    }
    std::cout << std::endl;
  }

  outputFile.close();
  return true;
}

void generateKnot(KnotVertex* k, int n) {
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

  k->printAll();


  #ifdef DEBUG
  std::cout << "number of crossings: " << numOcross << std::endl;
  #endif

  //failsafe:
  if (!generateNotation(k, numberOfCrossings)){
    free(k);
    KnotVertex * k = new KnotVertex;
    generateKnot(k, n);
  }
}
