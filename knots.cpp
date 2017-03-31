
#include <math.h>
#include <sstream> //for stringstream
#include <stdlib.h>
#include <sys/stat.h> //for fileExists
#include <time.h>
#include "knot.h"

#define NUMBEROFKNOTS 25
//a knot should be a linked list of nodes with an xval, a yval and if it has a crossing. There should be a list of crossings. Each crossing should have the index of the points of the line it crosses with and say if it's top or bottom.


//bool fileExists is from http://stackoverflow.com/questions/4316442/stdofstream-check-if-file-exists-before-writing
bool fileExists(const std::string& filename){
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1){
      return true;
  }
  return false;
}

std::string checkFileName (std::string fileName, std::string fileExt){
  if(fileExists(fileName + fileExt)){
    char answer;
    std::cout << "Would you like to overwrite the current " << fileName << fileExt << " file? (y/n) ";
    std::cin >> answer;
    if (answer != 'y' && answer != 'Y'){
      int i=0;
      while (fileExists(fileName + fileExt)){
        ++i;
        std::stringstream sstm;
        sstm << i;
        fileName = fileName + sstm.str();
      }
    }
  }
      
  std::cout << "\nSaving output file as " << fileName << fileExt << std::endl << std::endl;

  return fileName + fileExt;
}

int main(){
	srand(time(NULL));
  KnotVertex * knot;

  int n;
  std::string outputFileBaseName = "generatedKnots", outputFileExt = ".txt";

  std::string outputFileName = checkFileName(outputFileBaseName, outputFileExt);
  
  #ifdef DEBUG
  testKnot();
  #endif

  std::cout << "How many points would you like to generate for each knot? ";
  std::cin >> n;
  std::cout << std::endl;

  ofstream outputFile;

  outputFile.open(outputFileName.c_str(), std::ios_base::trunc);

  for(int i=0; i < NUMBEROFKNOTS; ++i){
    knot = new KnotVertex();
    generateKnot(knot, n, outputFile);
    outputFile << "\n\n";
    free(knot);
    knot = NULL;
  }
  outputFile.close();

  const char * fileNameToPass = outputFileName.c_str();

  const char * pass[] = {"lmpoly", fileNameToPass};

  std::string homflyOutputFileBaseName = "lmknot", homflyOutputFileExt = ".out";

  std::string homflyOutputFileName = checkFileName(homflyOutputFileBaseName, homflyOutputFileExt);

  int k = milletMain(2, pass, homflyOutputFileName.c_str());

  std::map<string, int> homOutCount =  parseHomflyOutput(homflyOutputFileName.c_str());

  for(map<string, int>::const_iterator it = homOutCount.begin(); it != homOutCount.end(); ++it){
    if (it->second == 1){
      std::cout << "There was " << it->second << "\n" << it->first << "knot." << std::endl
      << "This is " << 100.0*it->second/NUMBEROFKNOTS << "%" << std::endl << std::endl;
    }
    else{
      std::cout << "There were " << it->second << "\n" << it->first << "knots." << std::endl
      << "This is " << 100.0*it->second/NUMBEROFKNOTS << "%" << std::endl << std::endl;
    }
  }


}
