//21233016  32387364
// Lee and Tom 13.12.20

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

using namespace std;
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "anomaly_detection_util.h"

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
    Circle():center(0,0),radius(0){}
};

float distance(Point p1, Point p2);

Point circleCenter(Point p1, Point p2, Point p3);

/* Circle is (x-xc)^2 + (y-yc)^2 = R^2, thus with 3 points we can find center
 * x2^2 - x1^2 -2(x2-x1)xc + same for yc = 0
 * then same with x1, x3 and y1, y3
 * 2 linear equations
*/
Circle createCircle(Point p1, Point p2, Point p3);

// Create for 2 points
Circle createCircle(Point p1, Point p2);

bool isPointInsideCircle(Point p, Circle c);

Circle findTrivialSolution(vector<Point> points);

Circle findMinCircleHelper(Point** points, vector<Point> r, int size);

// Main func
Circle findMinCircle(Point** points,size_t size);


#endif /* MINCIRCLE_H_ */
