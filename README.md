# knot-generation-and-analysis

This project is designed to find the average shape of a Legendrian Knot. It does so in the following way:

1) Given an integer n, generate n random points. If it is a valid point, add it to the knot, else generate a new point.

2) Once the knot is generated, create an array of all of the crossings in Ewing/Millett notation.

3) Use this array to calculate the HOMFLY polynomial to classify the knot.

4) Repeat some large number of times, keeping track of the knot classification.

5) Output the percentage of each knot created

To use on a Linux machine:
-  `make`
-  `./run`
-  follow the prompts  
  
*Note that it will create multiple files for manual inspection/analysis - the final counts/percents will be in finalResults files and those including Bennequin will be in finalResultsBR files.*

TODO:
- add ability to generate by number of crossings
- clean up generateNotation in func.cpp
- change checkSameLine in func.cpp to be KnotVertex rather than knotNot based
- change checkIfV in func.cpp to be KnotVertex rather than knotNot based

Working:
- Knot-Generation
- Crossing Generation
- Crossing Labelling
- Notation Generation (?)
- Passing File to Homfly code
- parse homfly output
- compute percentage of each knot created
- add B/R notation/parsing 

Notation Generation:
- No-crossing knots
- One-crossing knots
- Unknots of most crossings
- Seems to be working - haven't run into any errors in quite a while

/****  
Sources:  
http://stackoverflow.com/questions/9878965/c-rand-between-0-and-1  
http://www.cplusplus.com/forum/beginner/49408/ (Disch's answer)  
http://stackoverflow.com/questions/14176776/find-out-if-2-lines-intersect  
http://www.codeproject.com/Tips/862988/Find-the-intersection-point-of-two-line-segments  
http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect  
http://stackoverflow.com/questions/20010909/how-to-loop-through-multiple-functions-in-c  
https://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible  
http://www.cplusplus.com/forum/beginner/27582/  
http://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line  
https://msdn.microsoft.com/en-us/library/7k66t42c.aspx  
http://stackoverflow.com/questions/4316442/stdofstream-check-if-file-exists-before-writing  
http://stackoverflow.com/questions/191757/how-to-concatenate-a-stdstring-and-an-int  
http://stackoverflow.com/questions/14070940/c-printing-out-map-values  
http://stackoverflow.com/questions/5056645/sorting-stdmap-using-value  
http://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string  
http://www.cplusplus.com/doc/tutorial/dynamic/  
*****/

*Special thanks to Charles and Harold for letting me talk at them about this and acting as sounding boards*
