#include <iostream>
#include "timeseries.h"

using namespace std;

void TimeSeries::print() const{
    auto it = data.begin();
    while (it != data.end()){
        cout<< it->first + ": " << endl;
        for (int i = 0; i < it->second.size(); i++){
            cout << it->second.at(i) << endl;
        }
        it++;
    }
}

const map<string,vector<float>>& TimeSeries::getData() const{
    return data;
}

const vector<pair<string, vector<float>>> TimeSeries::dataAsVector() const{
    auto it = data.begin();
    vector<pair<string, vector<float>>> vec;
    for (; it != data.end(); it++) {
        vec.push_back(*it);
    }
    return vec;
}

float TimeSeries::valueAtTimeStep(string param, long timeStep) const{
    return data.at(param).at(timeStep);
}

vector<string> TimeSeries::getParameterNames() const{
    return paramNames;
}

Point TimeSeries::getTimeStepPoint(string param1, string param2, long timeStep) const{
    return Point(valueAtTimeStep(param1, timeStep),valueAtTimeStep(param2, timeStep));
}

Point** TimeSeries::getAllPoints(string param1, string param2) const{
    int size = data.at(param1).size();
    Point** points = new Point*[size];
    for (int i = 0; i < size; i++){
        points[i] = new Point(getTimeStepPoint(param1, param2, i));
    }
    return points;
}