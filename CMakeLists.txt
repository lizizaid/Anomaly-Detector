cmake_minimum_required(VERSION 3.16)
project(AnomalyDetector)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Anomaly-Detector anomaly_detection_util.h anomaly_detection_util.cpp MainTrain.cpp AnomalyDetector.h timeseries.h timeseries.cpp SimpleAnomalyDetector.h SimpleAnomalyDetector.cpp minCircle.h minCircle.cpp HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.cpp CLI.h commands.h Server.cpp Server.h)