

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}
};

class SocketDio : public DefaultIO{
    int clientID;
public:
     SocketDio(int clientID) : clientID(clientID) {}
     string recv_until_newline(){
         string msg = "";
         int i = 0;
         char buffer[1];
         while (true) {
             if (recv(clientID, buffer, 1, 0) < 0)
                 throw "Failed to receive from client.";
             if (buffer[0] == '\n')
                 break;
             msg+=buffer[0];
             i++;
         }

         return msg;
     }
     string read() override{
       return recv_until_newline();
    }
    void read(float* f) override{
         string msg = recv_until_newline();
         *f = stof(msg);
     }
    void write(string text) override{
         char* msg = &text[0];
         int msg_len = strlen(msg);
         if (send(clientID, msg, msg_len, 0) < 0)
             throw "Failed to write to client.";
     }

    void write(float f) override{
        stringstream ss;
        ss << f;
        string msg_t(ss.str());
        char* msg = &msg_t[0];
        int msg_len = strlen(msg);
        if (send(clientID, msg, msg_len, 0) < 0)
            throw "Failed to write to client.";
    }
};

class Analyzer{
    SimpleAnomalyDetector* detector;
    int timeSteps;
    vector<AnomalyReport> anomalies;

public:
    Analyzer(SimpleAnomalyDetector* detector):detector(detector), timeSteps(0), anomalies(vector<AnomalyReport>{}){}

    float get_threshold(){return detector->get_threshold();}
    void set_threshold(float t){detector->set_threshold(t);}
    vector<AnomalyReport> getAnomalies(){return anomalies;}
    int getTimeSteps(){return timeSteps;}

    vector<pair<string, vector<float>>> read_data(DefaultIO* dio){
        vector<pair<string, vector<float>>> data;
        string line, parameterName;

        line = dio->read();
        stringstream stream(line);
        while (getline(stream, parameterName, ',' )){
            data.push_back(pair<string, vector<float>>(parameterName, vector<float>{}));
        }

        float val;
        while (true){
            line = dio->read();
            if (line.compare("done")==0)
                break;
            stringstream stream(line);
            int collumIndex = 0; // index of current collum
            while (stream >> val){
                data.at(collumIndex).second.push_back(val);
                if (stream.peek() == ',')
                    stream.ignore();
                collumIndex++;
            }
        }
        return data;
    }

    void write_to_csv(string file_name, vector<pair<string, vector<float>>> data){
        ofstream file(file_name);
        int col_amount = data.size();
        for (int k = 0; k < col_amount; k++){
            file << data.at(k).first;
            if (k != (col_amount - 1))
                file << ",";
        }
        file << "\n";

        for (int i = 0; i < data.at(0).second.size(); ++i){
            for (int j = 0; j < col_amount; ++j){
                file << data.at(j).second.at(i);
                if (j != (col_amount - 1))
                    file << ",";
            }
            file << "\n";
        }
        file.close();
    }

    void write_to_csv(string file_name, DefaultIO* dio){
        write_to_csv(file_name, read_data(dio));
    }

    void analyze(const char* train_file_name,const char* test_file_name){
        const TimeSeries train_ts(train_file_name);
        const TimeSeries test_ts(test_file_name);
        timeSteps = test_ts.getSize();
        detector->learnNormal(train_ts);
        anomalies = detector->detect(test_ts);
    }

    ~Analyzer(){delete detector;}
};


class Command{
protected:
    DefaultIO* dio;
    Analyzer* analyzer;
    string description;
public:
    Command(DefaultIO* dio, Analyzer* analyzer, string description):dio(dio), analyzer(analyzer), description(description){}
    virtual void execute()=0;
    virtual ~Command(){}
    string get_description(){return description;}
};


class UploadCsvFile : public Command{
public:
    UploadCsvFile(DefaultIO* dio, Analyzer* analyzer) : Command(dio,analyzer,"upload a time series csv file"){}

    void execute() override{
        dio->write("Please upload your local train CSV file.\n");
        analyzer->write_to_csv("anomalyTrain.csv", dio);
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        analyzer->write_to_csv("anomalyTest.csv", dio);
        dio->write("Upload complete.\n");
    }
};

class AlgoSettings : public Command{
public:
    AlgoSettings(DefaultIO* dio, Analyzer* analyzer) : Command(dio, analyzer, "algorithm settings"){}
    void execute() override{
        float new_t;
        bool legal_ts = false;
        while (!legal_ts){
            dio->write("The current correlation threshold is ");
            dio->write(analyzer->get_threshold());
            dio->write("\nType a new threshold\n");
            dio->read(&new_t);
            if (new_t >= 0 && new_t <= 1){
                analyzer->set_threshold(new_t);
                legal_ts = true;
            }
            else{
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class DetectAnomalies : public Command{
public:
    DetectAnomalies(DefaultIO* dio, Analyzer* analyzer) : Command(dio, analyzer, "detect anomalies"){}
    void execute() override{
        analyzer->analyze("anomalyTrain.csv", "anomalyTest.csv");
        dio->write("anomaly detection complete.\n");
    }
};

class DisplayResults : public Command{
public:
    DisplayResults(DefaultIO* dio, Analyzer* analyzer) : Command(dio,analyzer, "display results"){}
    void execute() override{
        vector<AnomalyReport> results = analyzer->getAnomalies();
        int size = results.size();
        for (int i = 0; i < size; i++){
            int timeStep = results.at(i).timeStep;
            dio->write(timeStep);
            dio->write("\t" + results.at(i).description + "\n");
        }
        dio->write("Done.\n");
    }
};

class AnalyzeResults : public Command{
public:
    AnalyzeResults(DefaultIO* dio, Analyzer* analyzer) : Command(dio,analyzer, "upload anomalies and analyze results"){}

    vector<pair<int, int>> unify_anomalies(){
        vector<AnomalyReport> anomalies = analyzer->getAnomalies();
        vector<pair<int,int>> unified;
        int size = anomalies.size();
        int start = 0;
        for (int i = 1; i < size; i++){
            if (anomalies[i].timeStep == anomalies[i-1].timeStep+1
            && anomalies[i].description == anomalies[start].description) {
                if (i == size-1){
                    unified.push_back(pair<int,int>(anomalies[start].timeStep,anomalies[i].timeStep));
                }
                continue;
            }
            else{
                unified.push_back(pair<int,int>(anomalies[start].timeStep,anomalies[i-1].timeStep));
                start = i;
                i++;
            }
        }
        return unified;
    }

    void execute() override{
        vector<pair<int,int>> reports{};
        string line;
        int num1, num2;

        dio->write("Please upload your local anomalies file.\n");
        while (true){
            line = dio->read();
            if (line == "done")
                break;
            int pos = line.find(",");
            if (pos==line.npos)
                continue;
            stringstream ss1(line.substr(0, pos));
            ss1 >> num1;
            stringstream ss2(line.substr(pos+1));
            ss2 >> num2;
            reports.push_back(pair<int, int>(num1, num2));
        }
        dio->write("Upload complete.\n");
        vector<pair<int, int>> anomalies = unify_anomalies();
        int anomalies_len = anomalies.size();
        int n = analyzer->getTimeSteps();
        int P = reports.size();
        int N = n;
        for (int i = 0; i < P; i++){
            int lines = reports.at(i).second - reports.at(i).first + 1;
            N-=lines;
        }

        int TP = 0, FP;
        for (int j = 0; j < P; j++){
            pair<int,int> report = reports[j];
            for (int k =0; k < anomalies_len; k++){
                pair<int, int> anomaly = anomalies[k];
                if ((report.first >= anomaly.first && report.first <= anomaly.second)
                ||(report.second <= anomaly.second && report.second >= anomaly.first)
                ||(report.first <= anomaly.first && report.second >= anomaly.second)){
                    TP++;
                    break;
                }
            }
        }
        FP = anomalies_len - TP;
        float TP_rate = float(TP) / float(P);
        TP_rate*=1000;
        TP_rate = floor(TP_rate);
        TP_rate/=1000;
        float FP_rate = float(FP) / float(N);
        FP_rate*=1000;
        FP_rate = floor(FP_rate);
        FP_rate/=1000;
        dio->write("True Positive Rate: ");
        dio->write(TP_rate);
        dio->write("\nFalse Positive Rate: ");
        dio->write(FP_rate);
        dio->write("\n");

    }
};

class Exit : public Command{
public:
    Exit(DefaultIO* dio, Analyzer* analyzer) : Command(dio,analyzer, "exit"){}
    void execute() override{}
};



#endif /* COMMANDS_H_ */
