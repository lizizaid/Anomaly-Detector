#include "SimpleAnomalyDetector.h"
#include <iostream>
using namespace std;
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    vector<pair<string, vector<float>>> data = ts.dataAsVector();
    int size = data.size();
    int sizeOfVector = data.at(0).second.size();
    for (int i = 0; i < size - 1; i++) {
        float maxCor = -1;
        float *x = new float[sizeOfVector];
        for (int k = 0; k < sizeOfVector; k++){
            x[k] = data.at(i).second.data()[k];
        }
        string bestCorrelated;
        for (int j = i + 1; j < size; j++) {
            float *y = new float[sizeOfVector];
            for (int k = 0; k < sizeOfVector; k++){
                y[k] = data.at(j).second.data()[k];
            }
            float cor = fabs(pearson(x, y, sizeOfVector));
            if (cor > maxCor) {
                maxCor = cor;
                bestCorrelated = data.at(j).first;
            }
            delete y;
        }
        string firstFeature = data.at(i).first;
        Point **points = ts.getAllPoints(firstFeature, bestCorrelated);
        size_t len = data.at(i).second.size();
        create_correlated_features(firstFeature, bestCorrelated, maxCor, points, len);
        // Delete points
        for (int i = 0; i < size; i++) {
            delete points[i];
        }
        delete[] points;
        delete[] x;
    }
}
void SimpleAnomalyDetector::create_correlated_features(string f1, string f2, float cor, Point** points, size_t len){
    if (cor > threshold) {
        correlatedFeatures cofeat;
        cofeat.feature1 = f1;
        cofeat.feature2 = f2;
        cofeat.corrlation = cor;
        cofeat.lin_reg = linear_reg(points, len);
        float th = find_threshold(points, len, cofeat);
        cofeat.threshold = th * 1.1;
        cf.push_back(cofeat);
    }
}

float SimpleAnomalyDetector::find_threshold(Point** ps,size_t len, correlatedFeatures cof){
    float max=0;
    Line rl = cof.lin_reg;
    for(size_t i=0;i<len;i++){
        float d=abs(ps[i]->y - rl.f(ps[i]->x));
        if(d>max)
            max=d;
    }
    return max;
}

bool SimpleAnomalyDetector::is_anomaly(Point p, correlatedFeatures c){
    float d = dev(p, c.lin_reg);
    return d > c.threshold;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reports;
    for (int i = 0; i < cf.size(); i++){
        int size = ts.getData().at(cf[i].feature1).size();
        for (int j = 0; j < size; j++){
            Point point = ts.getTimeStepPoint(cf[i].feature1, cf[i].feature2, j);
            if (is_anomaly(point,cf[i])){
                string description = cf[i].feature1 + "-" + cf[i].feature2;
                AnomalyReport report(description, j+1);
                reports.push_back(report);
            }
        }
    }
    return reports;
}

