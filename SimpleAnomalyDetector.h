

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Circle min_circle;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> cf;
    float threshold;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    float get_threshold() const {return threshold;}
    void set_threshold(float t){threshold = t;}
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual float find_threshold(Point** ps,size_t len, correlatedFeatures cof);
    virtual void create_correlated_features(string f1, string f2, float cor, Point** points, size_t len);
    virtual bool is_anomaly(Point p, correlatedFeatures c);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
