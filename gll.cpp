/*
 * Samantha Kacir
 * The purpose of this is to create a Circular List class for holding knots
 * and the functions needed to work with it.
*/

#include "knot.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

int main() {

 GenericList pos = GenericList();

#ifdef TEST
 pos.add(1);
 //pos.printAll();
 pos.add((float)20.0);
 //pos.printAll();
 pos.add((float)20.0);
 //pos.printAll();
 pos.add('N');
 //pos.printAll();
 pos.add(30);
 //pos.printAll();
 pos.add('W');
 //pos.printAll();
 pos.add(34);
 //pos.printAll();
 pos.add("positionOne");
 //pos.printAll();

 //pos.print(100);
 //pos.printAll();
 
 GenericList test = GenericList();
 GenericList * p = new GenericList();
 //pos.remove();
 *p = pos;

 test.add(p);
 
 /* while(pos.getData() != NULL){
   pos.remove();
   }*/
 GenericList sop = GenericList();
 sop.add(2);
 //pos.printAll();
 sop.add((float)33.0);
 //pos.printAll();
 sop.add((float)-200.0);
 //pos.printAll();
 sop.add('S');
 //pos.printAll();
 sop.add(100);
 //pos.printAll();
 sop.add('E');
 //pos.printAll();
 sop.add(10);
 //pos.printAll();
 sop.add("positionTwo");
 //pos.printAll();

 GenericList* s = new GenericList();
 *s = sop;
 test.add(s); 
 test.printAll();

#else
 
 GenericList gl = GenericList();
  char token[256];
  char ptok[256];
  char* piece;
  char** data;
  int count = 0;
  ifstream datdat("lab08test.csv");
  
  while(! datdat.getline(token, 256).eof()){
    //for each line create a genericList
    GenericList* t = new GenericList();
    GenericList temp = GenericList();
    ++count;
    piece = strtok(token, " ,\t");
    int cc = 0;
    while(piece != NULL){
      //cout<<count<<endl;
      if(cc == 0 || cc == 4 || cc==6){
	temp.add(atoi(piece));
      }
	else if(cc==1 || cc==2){
	  temp.add((float)atof(piece));
	}
	else if(cc==3 || cc==5){
	  temp.add(piece[0]);
	}
	else if(cc == 7){
	  temp.add(string(piece));
	}
      
      ++cc;
      
      piece = strtok(NULL, " ,\t");
    }
    
    temp.printAll();
    *t = temp;
    //add temp to gl
    gl.add(t);
  }
  //cout<<count<<endl;
  //gl.print(0);
  gl.printAll();
  
#endif
  
  return 0;
  
}
