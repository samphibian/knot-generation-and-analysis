
#include <math.h>
#include <sstream> //for stringstream
#include <stdlib.h>
#include <sys/stat.h> //for fileExists
#include <time.h>
#include "knot.h"

//a knot should be a linked list of nodes with an xval, a yval and if it has a crossing. There should be a list of crossings. Each crossing should have the index of the points of the line it crosses with and say if it's top or bottom.


//bool fileExists is from http://stackoverflow.com/questions/4316442/stdofstream-check-if-file-exists-before-writing
bool fileExists(const std::string& filename){
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1){
      return true;
  }
  return false;
}

int main(){
	srand(time(NULL));
  KnotVertex * knot;

  int n;
  std::string outputFileName = "generatedKnots.txt";

  if(fileExists(outputFileName)){
    char answer;
    std::cout << "Would you like to overwrite the current " << outputFileName << " file? (y/n)";
    std::cin >> answer;
    if (answer != 'y' && answer != 'Y'){
      int i = 0;
      while (fileExists(outputFileName)){
        ++i;
        std::stringstream sstm;
        sstm << i;
        outputFileName = "generatedKnots" + sstm.str() + ".txt";
      }
      std::cout << "\nSaving file as " << outputFileName << std::endl << std::endl;
    }
  }
  
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
    //outputFile << i+1 << ". ";
    generateKnot(knot, n, outputFile);
    outputFile << "\n\n";
    free(knot);
    knot = NULL;
  }
  outputFile.close();
}
