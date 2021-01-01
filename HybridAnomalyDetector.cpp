//
// Created by Lee on 01/01/2021.
//

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

float HybridAnomalyDetector::find_threshold(Point** ps,size_t len,correlatedFeatures cof){
    if (cof.corrlation > 0.9){
        return SimpleAnomalyDetector::find_threshold(ps, len, cof);
    }
    return cof.min_circle.radius;
}

void HybridAnomalyDetector::create_correlated_features(string f1, string f2, float cor, Point** points, size_t len){
    if (cor > threshold) {
        SimpleAnomalyDetector::create_correlated_features(f1,f2,cor,points,len);
    }
    else if (cor > 0.5){
        correlatedFeatures cofeat;
        cofeat.feature1 = f1;
        cofeat.feature2 = f2;
        cofeat.corrlation = cor;
        cofeat.min_circle = findMinCircle(points, len);
        float th = find_threshold(points, len, cofeat);
        cofeat.threshold = th * 1.1;
        cf.push_back(cofeat);
    }
}


bool HybridAnomalyDetector::is_anomaly(Point p, correlatedFeatures c){
    if (c.corrlation > 0.9){
        return SimpleAnomalyDetector::is_anomaly(p, c);
    }
    return distance(p, c.min_circle.center) > c.threshold;
}