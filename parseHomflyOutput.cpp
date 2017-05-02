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
  std::map<string,int> outputMap;

  ifstream homflyOutput;

    homflyOutput.open(parseFileName);

    string line;

    ifstream br;
    br.open("storeBR.txt");

    string brNot;

    ifstream gk;
    gk.open("generatedKnots.txt");

    string genKnot;

    while(std::getline(homflyOutput, line)){
      int gkLineCount = 0;
      std::getline(gk, genKnot);
      if(genKnot != "") std::getline(gk, genKnot);

      while(genKnot == "" && gkLineCount < 3){
        std::getline(gk, genKnot);
        ++gkLineCount;
      }

      std::getline(br, brNot);
      if (brNot == "0 0" && gkLineCount == 3){
        string key = brNot +"\n[[1]]\n";
        if (outputMap[key]) {
          ++outputMap[key];
        }
        else {
          outputMap[key]=1;
        }
      }

      else {
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
      }
    }

    homflyOutput.close();
    br.close();
    gk.close();

    #ifdef DEBUG
    for(map<string, int>::const_iterator it = outputMap.begin(); it != outputMap.end(); ++it){
        std::cout << it->first << " " << it->second << std::endl;
    }
    #endif

  return outputMap;
}

// void printMap(std::map<string, int> mapToSort, int totalNumberOfKnots){
//   // code between /**/ from http://thispointer.com/how-to-sort-a-map-by-value-in-c/
//   /**/
//   // Declaring the type of Predicate that accepts 2 pairs and return a bool
//   typedef std::function<bool(std::pair<string, int>, std::pair<string, int>)> Comparator;
 
//   // Defining a lambda function to compare two pairs. It will compare two pairs using second field
//   Comparator compFunctor =
//       [](std::pair<string, int> elem1 ,std::pair<string, int> elem2)
//       {
//         return elem1.second <= elem2.second;
//       };
 
//   // Declaring a set that will store the pairs using above comparision logic
//   std::set<std::pair<std::string, int>, Comparator> sortedSet(
//       mapToSort.begin(), mapToSort.end(), compFunctor);
//   /**/

//   // #ifdef DEBUG
//   std::cout << mapToSort.size() << " " << sortedSet.size() << std::endl;
//   // #endif

//   for (auto element : sortedSet){
//     // std::cout << element.second << " " << element.first << std::endl;
//     if (element.second == 1){
//       std::cout << "There was " << element.second << "\n" << element.first << "knot." << std::endl
//       << "This is " << 100.0*element.second/totalNumberOfKnots << "%" << std::endl << std::endl;
//     }
//     else{
//       std::cout << "There were " << element.second << "\n" << element.first << "knots." << std::endl
//       << "This is " << 100.0*element.second/totalNumberOfKnots << "%" << std::endl << std::endl;
//     }
//   }

//   //   set<unsigned long>::iterator it;
//   // for (it = sortedSet.begin(); it != sortedSet.end(); it++){

//   //   std::cout << *it.second << " " << *it.first << std::endl;
//   //   if (*it.second == 1){
//   //     std::cout << "There was " << *it.second << "\n" << *it.first << "knot." << std::endl
//   //     << "This is " << 100.0*(*it.second/totalNumberOfKnots << "%" << std::endl << std::endl;
//   //   }
//   //   else{
//   //     std::cout << "There were " << *it.second << "\n" << *it.first << "knots." << std::endl
//   //     << "This is " << 100.0*sortedSet.get(i).second/totalNumberOfKnots << "%" << std::endl << std::endl;
//   //   }
//   // }
// }

// void makeMap(std::map<string, int> mapToSort, int totalNumberOfKnots){
//   for(map<string, int>::const_iterator it = homOutCount.begin(); it != homOutCount.end(); ++it){
//     if (it->second == 1){
//       std::cout << "There was " << it->second << "\n" << it->first << "knot." << std::endl
//       << "This is " << 100.0*it->second/NUMBEROFKNOTS << "%" << std::endl << std::endl;
//     }

//     else{
//       std::cout << "There were " << it->second << "\n" << it->first << "knots." << std::endl
//       << "This is " << 100.0*it->second/NUMBEROFKNOTS << "%" << std::endl << std::endl;
//     }
//   }
// }

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

void printMap(std::map<string, int> mapToSort, int totalNumberOfKnots)
{
   std::multimap<int, string> sortedMap = flip_map(mapToSort);

  for(map<int, string>::const_iterator it = sortedMap.begin(); it != sortedMap.end(); ++it){
    if (it->first == 1){
      std::cout << "There was " << it->first << "\n" << it->second << "knot." << std::endl
      << "This is " << 100.0*it->first/totalNumberOfKnots << "%" << std::endl << std::endl;
    }

    else{
      std::cout << "There were " << it->first << "\n" << it->second << "knots." << std::endl
      << "This is " << 100.0*it->first/totalNumberOfKnots << "%" << std::endl << std::endl;
    }
  }
}