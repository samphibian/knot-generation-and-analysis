# knot-generation-and-analysis

This project is designed to find the average shape of a Legendrian Knot. It does so in the following way:

1) Given an integer n, generate n random points. If it is a valid point, add it to the knot, else generate a new point.

2) Once the knot is generated, create a vector of all of the crossings in Ewing/Millett notation.

3) Use this vector to calculate the HOMFLY polynomial to classify the knot.

4) Repeat some large number of times, keeping track of the knot classification.

5) Output the percentage of each knot created.

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
*****/