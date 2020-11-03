/*
 * anomaly_detection_util.cpp
 *
 * Author: Lee Zaid 212330161
 */

#include <math.h>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float sum = 0;
    for (int i = 0; i<size; i++){
        sum += x[i];
    }
    return sum/size;
}

// returns the variance of X and Y
float var(float* x, int size){
    float expVal = pow(avg(x,size),2);
    float xSquare [size];
    for (int i = 0; i < size; i++){
        xSquare[i] = pow(x[i],2);
    }
    return avg(xSquare,size) - expVal;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float xy [size];
    for (int i = 0; i < size; i++){
        xy[i] = x[i]*y[i];
    }
    return avg(xy,size) - avg(x,size) * avg(y,size);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    float stdDevX = sqrt(var(x,size));
    float stdDevY = sqrt(var(y,size));
    return cov(x,y,size)/(stdDevX*stdDevY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float x [size];
    float y [size];
    for (int i = 0; i< size; i++){
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x,y,size)/var(x,size);
    float b = avg(y,size) - a*avg(x,size);
    return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line line = linear_reg(points,size);
    return dev(p,line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float dev = l.f(p.x) - p.y;
    if (dev < 0){
        return -1*dev;
    }
    return dev;
}




