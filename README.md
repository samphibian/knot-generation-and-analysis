# knot-generation-and-analysis

This project is designed to find the average shape of a Legendrian Knot. It does so in the following way:
1) Given an integer n, generate n random points. If it is a valid point, add it to the knot, else generate a new point.
2) Once the knot is generated, create a vector of all of the crossings in Ewing/Millett notation.
3) Use this vector to calculate the HOMFLY polynomial to classify the knot.
4) Repeat some large number of times, keeping track of the knot classification.
5) Output the percentage of each knot created.
