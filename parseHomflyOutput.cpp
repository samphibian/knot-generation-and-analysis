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

#include <functional>
/*
For the oriented polynomial, if the polynomial
for two distant circles can be written as:
  mu = (l + l^-1)/m
then the powers of l are written horizontally, most negative to the left, and
the powers of m are written vertically, with the most negative powers toward
the beginning of the file (printed first).  A pair of square brackets surrounds
the zero power of both variables everywhere they occur.
*/

std::map<string, int> parseHomflyOutput(const char * parseFileName, int totalNumberOfKnots){
  //if totalNumberOfKnots < number parse, add difference to total number of unknots

  int knotCount = 0;

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
      ++knotCount;
    }

    homflyOutput.close();

    #ifdef DEBUG
    for(map<string, int>::const_iterator it = outputMap.begin(); it != outputMap.end(); ++it){
        std::cout << it->first << " " << it->second << std::endl;
    }
    #endif

    if (knotCount < totalNumberOfKnots){
      outputMap["[[1]]\n"] += (totalNumberOfKnots - knotCount);
    }

  return outputMap;
}

std::map<string, int> parseHomflyBROutput(const char * parseFileName, int totalNumberOfKnots, std::string fileSuffix){
  std::map<string,int> outputMap;
  int knotCount = 0;

  ifstream homflyOutput;

    homflyOutput.open(parseFileName);

    string line;

    ifstream br;
    br.open("storeBR" + fileSuffix +".txt");

    string brNot;

    while(std::getline(homflyOutput, line)){
      std::getline(br, brNot);

      string totalLine = brNot + "\n";

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
      ++knotCount;
    }
    homflyOutput.close();

    while(std::getline(br, brNot)){
      string noCross = brNot+"\n[[1]]";
      if(outputMap[noCross]){
        outputMap[noCross]++;
      }
      else{
        outputMap[noCross] = 1;
      }
      ++knotCount;
    }
    br.close();

    ifstream brNoCross;
    brNoCross.open("storeBR" + fileSuffix + "-NoCrossing" + ".txt");

    while(std::getline(brNoCross, line)){
      string noCross = line+"\n[[1]]";
      if(outputMap[noCross]){
        outputMap[noCross]++;
      }
      else{
        outputMap[noCross] = 1;
      }
      ++knotCount;
    }
    brNoCross.close();

    #ifdef DEBUG
    for(map<string, int>::const_iterator it = outputMap.begin(); it != outputMap.end(); ++it){
        std::cout << it->first << " " << it->second << std::endl;
    }
    #endif

  return outputMap;
}

//everything between /**/ from http://stackoverflow.com/questions/5056645/sorting-stdmap-using-value (Oliver Charlesworth's answer)
/**/
template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
                   flip_pair<A,B>);
    return dst;
}

/**/

void printMap(std::map<string, int> mapToSort, int totalNumberOfKnots, const char * fileOutputName)
{
   std::multimap<int, string> sortedMap = flip_map(mapToSort);


  ofstream outputFile;

  outputFile.open(fileOutputName);

  for(map<int, string>::const_iterator it = sortedMap.begin(); it != sortedMap.end(); ++it){
    float percent = (float)100.0*(float)it->first/(float)totalNumberOfKnots;
    std::string oneLine = it->second;
    replace(oneLine.begin(), oneLine.end(), '\n', ' ');
    outputFile << it->first << "," << oneLine << "," << percent << "\n";

    if (it->first == 1){
      std::cout << "There was " << it->first << "\n" << it->second << " knot." << std::endl
      << "This is " << percent << "%" << std::endl << std::endl;
    }

    else{
      std::cout << "There were " << it->first << "\n" << it->second << " knots." << std::endl
      << "This is " << percent << "%" << std::endl << std::endl;
    }
  }
  outputFile.close();
}