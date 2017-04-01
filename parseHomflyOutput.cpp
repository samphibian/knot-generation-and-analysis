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

