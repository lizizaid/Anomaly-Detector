

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include "anomaly_detection_util.h"

using namespace std;

class TimeSeries{
    vector<string> paramNames;
    map<string,vector<float>> data;

public:
    TimeSeries(const char* CSVfileName){
        ifstream myFile;
        myFile.open(CSVfileName);
        if (!myFile.is_open())
            throw runtime_error("File could not open.");
        string line, parameterName;
        getline(myFile, line); // Get first line in file
        stringstream stream(line);
        while (getline(stream, parameterName, ',' )){
            data.insert(pair<string, vector<float>>(parameterName, vector<float>{}));
            paramNames.push_back(parameterName);
        }
        float val;
        while (getline(myFile,line)){
            stringstream stream(line);
            int collumIndex = 0; // index of current collum
            while (stream >> val){
                data.at(paramNames.at(collumIndex)).push_back(val);
                if (stream.peek() == ',')
                    stream.ignore();
                collumIndex++;
            }
        }
        myFile.close();
    }

    const map<string,vector<float>>& getData() const;
    const vector<pair<string, vector<float>>> dataAsVector() const;
    float valueAtTimeStep(string param, long timeStep) const;
    vector<string> getParameterNames() const;
    void print() const;
    Point getTimeStepPoint(string param1, string param2, long timeStep) const;
    Point** getAllPoints(string param1, string param2) const;

    ~TimeSeries(){}

};



#endif /* TIMESERIES_H_ */
