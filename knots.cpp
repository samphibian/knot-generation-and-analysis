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

#include <math.h>
#include <sstream> //for stringstream
#include <stdlib.h>
#include <sys/stat.h> //for fileExists
#include <time.h>
#include "knot.h"

#define NUMBEROFKNOTS 1000
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
      string curFileName = fileName;
      while (fileExists(curFileName + fileExt)){
        ++i;
        std::stringstream sstm;
        sstm << i;
        curFileName = fileName + sstm.str();
      }
      fileName = curFileName;
    }
    else if (answer == 'y'){
      remove ((fileName + fileExt).c_str());
    }
  }
      
  std::cout << "\nSaving output file as " << fileName << fileExt << std::endl << std::endl;

  return fileName + fileExt;
}

std::string getFileSuffix (std::string fileName, std::string fileExt){
  std::string ret = "x";
  if(fileExists(fileName + fileExt)){
    char answer;
    std::cout << "Would you like to overwrite the current " << fileName << fileExt << " files? (y/n) ";
    std::cin >> answer;
    if (answer != 'y' && answer != 'Y'){
      int i=0;
      string curFileName = fileName;
      while (fileExists(curFileName + fileExt)){
        ++i;
        std::stringstream sstm;
        sstm << i;
        ret = sstm.str();
        curFileName = fileName + ret;
      }
      std::cout << "\nSaving output files with the suffix " << i << ". Ex: " << curFileName << fileExt << std::endl << std::endl;
      return ret;
    }
    else if (answer == 'y'){
      return ret;
    }
  }
}

int main(){
	srand(time(NULL));
  KnotVertex * knot;
  bool br;

  std::cout << "\nknot-generation-and-analysis Copyright (C) 2017 Samantha Kacir\nThis program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under certain conditions."
    << std::endl << std::endl;
  int n;
  std::string outputFileBaseName = "generatedKnots", outputFileExt = ".txt";

  std::string homflyOutputFileBaseName = "lmknot", homflyOutputFileExt = ".out";

  string suffix = getFileSuffix(outputFileBaseName, outputFileExt);

  char ans;
  std::cout << "Would you like to generate the tb and r as well? (y/n) ";
  std::cin >> ans;
  if(ans == 'y') br = true;
  else br = false;
  std::cout << std::endl;

  if(suffix == "x"){
    remove((outputFileBaseName+outputFileExt).c_str());
    remove((homflyOutputFileBaseName+homflyOutputFileExt).c_str());
    if (br) remove("storeBR.txt");
    suffix = "";
  }
  std::string outputFileName = outputFileBaseName + suffix + outputFileExt;

  #ifdef DEBUG
  testKnot();
  #endif

  std::cout << "How many points would you like to generate for each knot? ";
  std::cin >> n;
  if (n < 3){
    std::cerr << "Cannot create a knot with " << n << " vertices. Good-bye." << std::endl;
    exit(1);
  }
  std::cout << std::endl;

  ofstream outputFile;

  outputFile.open(outputFileName.c_str());

  for(int i=0; i < NUMBEROFKNOTS; ++i){
    knot = new KnotVertex();
    generateKnot(knot, n, outputFile, br, suffix);
    outputFile << "\n\n";
    free(knot);
    knot = NULL;
  }
  outputFile.close();

  const char * fileNameToPass = outputFileName.c_str();

  const char * pass[] = {"lmpoly", fileNameToPass};

  std::string homflyOutputFileName = homflyOutputFileBaseName + suffix + homflyOutputFileExt;

  int k = milletMain(2, pass, homflyOutputFileName.c_str());

  std::string finalOutputName;
  finalOutputName = "finalResultsBR" + suffix + ".csv";

  std::map<string, int> homOutCount;
  if (br) {
    homOutCount =  parseHomflyBROutput(homflyOutputFileName.c_str(), NUMBEROFKNOTS, suffix);
    printMap(homOutCount, NUMBEROFKNOTS, finalOutputName.c_str());
    std::cout << "Do you want to see the results without the Thurston-Bennequin notation? (y/n) ";
    std::cin >> ans;
    if (ans == 'n') exit(0);
  }
  finalOutputName = "finalResults" + suffix + ".csv";

  homOutCount = parseHomflyOutput(homflyOutputFileName.c_str(), NUMBEROFKNOTS);
  printMap(homOutCount, NUMBEROFKNOTS, finalOutputName.c_str());
}
