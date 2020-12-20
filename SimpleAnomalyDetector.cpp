#include "SimpleAnomalyDetector.h"
#include <iostream>
using namespace std;
SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    const map<string, vector<float>> m = ts.getData();
    auto it = m.begin();
    vector<pair<string, vector<float>>> data;
    for (; it != m.end(); it++) {
        data.push_back(*it);
    }
    int size = data.size();
    int sizeOfVector = data.at(0).second.size();
    for (int i = 0; i < size - 1; i++) {
        float max = -1;
        float *x = new float[sizeOfVector];
        for (int k = 0; k < sizeOfVector; k++){
            x[k] = data.at(i).second.data()[k];
        }
        string bestCorrelated = "";
        for (int j = i + 1; j < size; j++) {
            float *y = new float[sizeOfVector];
            for (int k = 0; k < sizeOfVector; k++){
                y[k] = data.at(j).second.data()[k];
            }
            float cor = fabs(pearson(x, y, sizeOfVector));
            if (cor > max && cor > 0.9) {
                max = cor;
                bestCorrelated = data.at(j).first;
            }
            delete y;
        }
        if (max != -1) {
            correlatedFeatures cofeat;
            cofeat.feature1 = data.at(i).first;
            cofeat.feature2 = bestCorrelated;
            cofeat.corrlation = max;
            Point **points = ts.getAllPoints(cofeat.feature1, cofeat.feature2);
            cofeat.lin_reg = linear_reg(points, data.at(i).second.size());
            float maxDev = -1, currentDev;
            for (int q = 0; q < data.at(i).second.size(); q++) {
                currentDev = dev(*points[q], cofeat.lin_reg);
                if (currentDev > maxDev)
                    maxDev = currentDev;
            }
            cofeat.threshold = maxDev*1.1;
            cf.push_back(cofeat);
            // Delete points
            for (int i = 0; i < size; i++) {
                delete points[i];
            }
            delete[] points;
        }
        delete x;
    }
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reports;
    for (int i = 0; i < cf.size(); i++){
        int size = ts.getData().at(cf[i].feature1).size();
        for (int j = 0; j < size; j++){
            Point point = ts.getTimeStepPoint(cf[i].feature1, cf[i].feature2, j);
            float d = dev(point, cf[i].lin_reg);
            if (d > cf[i].threshold){
                string description = cf[i].feature1 + "-" + cf[i].feature2;
                AnomalyReport report(description, j+1);
                reports.push_back(report);
            }
        }
    }
    return reports;
}

