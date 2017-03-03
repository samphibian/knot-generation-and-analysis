
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "knot.h"

//a knot should be a linked list of nodes with an xval, a yval, (or create point class - probably better. Point: double x, double y) and if it has a crossing. There should be a list of crossings. Each crossing should have the index of the points of the line it crosses with and say if it's top or bottom.

int main(){
	srand(time(NULL));
  KnotVertex * knot;

  int n;
  std::string outputFileName = "generatedKnots.txt";
  
  #ifdef DEBUG
  testKnot();
  #endif

  std::cout << "How many points would you like to generate for the knot? ";
  std::cin >> n;
  std::cout << std::endl;

  ofstream outputFile;

  outputFile.open(outputFileName.c_str(), std::ios_base::app);
  for(int i=0; i<10; ++i){
    knot = new KnotVertex();
    // outputFile << i+1 << ". ";
    generateKnot(knot, n, outputFile);
    outputFile << "\n";
    free(knot);
    knot = NULL;
  }
  outputFile.close();
}
