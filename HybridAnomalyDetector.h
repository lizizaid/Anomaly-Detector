//
// Created by Lee on 01/01/2021.
//


#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual float find_threshold(Point** ps,size_t len, correlatedFeatures cof);
    virtual void create_correlated_features(string f1, string f2, float cor, Point** points, size_t len);
    virtual bool is_anomaly(Point p, correlatedFeatures c);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
