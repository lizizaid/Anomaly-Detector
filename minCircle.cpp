//
// Created by Lee on 01/01/2021.
//

#include "minCircle.h"
float distance(Point p1, Point p2){
    return sqrt(pow((p1.x-p2.x),2) + pow((p1.y-p2.y),2));
}

Point circleCenter(Point p1, Point p2, Point p3){
    float x1 = (p2.x - p1.x), y1 = (p2.y - p1.y), x2 = (p3.x - p1.x), y2 = (p3.y - p1.y);
    float f1 = pow(x1,2) + pow(y1,2);
    float f2 = pow(x2,2) + pow(y2, 2);
    float f3 = x1*y2 - y1*x2;

    float xCenter = (y2*f1 - y1*f2)/(2*f3);
    float yCenter = (x1*f2 - x2*f1)/(2*f3);
    return {xCenter, yCenter};
}
/* Circle is (x-xc)^2 + (y-yc)^2 = R^2, thus with 3 points we can find center
 * x2^2 - x1^2 -2(x2-x1)xc + same for yc = 0
 * then same with x1, x3 and y1, y3
 * 2 linear equations
*/
Circle createCircle(Point p1, Point p2, Point p3){
    Point center = circleCenter(p1,p2,p3);
    center.x+=p1.x;
    center.y+=p1.y;
    float r = distance(center, p1);
    return {center,r};
}

// Create for 2 points
Circle createCircle(Point p1, Point p2){
    Point center = {(p1.x+p2.x)/2, (p1.y+p2.y)/2};
    float r = distance(p1, p2)/2;
    return {center, r};
}

bool isPointInsideCircle(Point p, Circle c){
    return distance(p, c.center) <= c.radius;
}

Circle findTrivialSolution(vector<Point> points){
    if (points.empty()) return {{0,0},0}; // No points, no circle
    else if (points.size() == 1) return {points[0], 0};
    else if (points.size() == 2) return createCircle(points[0], points[1]);
    else { // size == 3
        for (int i = 0; i < 3; i++){
            for (int  j = i+1; j < 3; j++){
                Circle c = createCircle(points[i], points[j]);
                bool flag = true;
                for (Point p : points){
                    if (!isPointInsideCircle(p, c)) flag = false;
                }
                if (flag) return c;
            }
        }
        return createCircle(points[0], points[1], points[2]);
    }
}

Circle findMinCircleHelper(Point** points, vector<Point> r, int size){
    if (size == 0 || r.size() == 3){
        return findTrivialSolution(r);
    }
    int newSize = size-1;
    Point* randomPoint = points[newSize];
    Circle c = findMinCircleHelper(points, r, size-1);
    if (isPointInsideCircle(*randomPoint, c)) return c;
    r.push_back(*randomPoint);
    return findMinCircleHelper(points, r, size-1);

}

// implement
Circle findMinCircle(Point** points,size_t size){
    return findMinCircleHelper(points, {}, size);
}


