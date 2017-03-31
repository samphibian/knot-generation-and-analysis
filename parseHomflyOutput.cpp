//The purpose of this file is to define the functions necessary to parse the Homfly output

#include "knot.h"

/*
For the oriented polynomial, if the polynomial
for two distant circles can be written as:
  mu = (l + l^-1)/m
then the powers of l are written horizontally, most negative to the left, and
the powers of m are written vertically, with the most negative powers toward
the beginning of the file (printed first).  A pair of square brackets surrounds
the zero power of both variables everywhere they occur.
*/

std::map<string, int> parseHomflyOutput(const char * parseFileName){
  //if totalNumberOfKnots < number parse, add difference to total number of unknots

  int unknot = 0;

  std::map<string,int> outputMap;

  ifstream homflyOutput;

    homflyOutput.open(parseFileName);

    string line;

    while(std::getline(homflyOutput, line)){
      string totalLine = "";

      while (line == "") std::getline(homflyOutput, line);

      while (line != ""){
        totalLine += line + "\n";
        std::getline(homflyOutput, line);
      }

      if(outputMap[totalLine]){
        outputMap[totalLine]++;
      }

      else{
        outputMap[totalLine] = 1;
      }
    }

    homflyOutput.close();

    #ifdef DEBUG
    for(map<string, int>::const_iterator it = outputMap.begin(); it != outputMap.end(); ++it){
        std::cout << it->first << " " << it->second << std::endl;
    }
    #endif

  return outputMap;
}

