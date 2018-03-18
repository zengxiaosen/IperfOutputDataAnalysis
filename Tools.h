//
// Created by root on 18-3-18.
//
#include <string>
#include <queue>
#include "zbInfoNode.h"

#ifndef IPERFOUTPUTDATAANALYSIS_TOOLS_H
#define IPERFOUTPUTDATAANALYSIS_TOOLS_H
using namespace std;

class Tools {
public:
    void readQueueClient(queue<string> q);
    void readQueue(queue<string> &queue);
    map<double, zbInfoNode, less<double>>  ReadDataFromFileLBLIntoCharArray(string s);\
    double ReadClientDataFromFileToDouble(string s);
};


#endif //IPERFOUTPUTDATAANALYSIS_TOOLS_H
