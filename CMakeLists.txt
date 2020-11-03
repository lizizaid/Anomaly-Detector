cmake_minimum_required(VERSION 3.16)
project(AnomalyDetector)

set(CMAKE_CXX_STANDARD 11)

add_executable(Anomaly-Detector anomaly_detection_util.h anomaly_detection_util.cpp MainTrain.cpp)