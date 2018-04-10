//
// Created by root on 18-4-10.
//

#ifndef IPERFOUTPUTDATAANALYSIS_GDATA_H
#define IPERFOUTPUTDATAANALYSIS_GDATA_H

#include <iostream>
#include <string>
#include <atomic>
#include "SyncQueue.hpp"

using namespace std;
class LogData{
public:


    LogData(){

    }

    ~LogData(void){
        //如果没有停止时则主动停止线程池
        //Stop();
    }

    void AddClientLog(string log){
        test ++;
        clientlog = log;

    }
    void AddServerLog(string log){
        test ++;
        serverlog = log;

    }


    void ReadLog(){

        int lastTime = 0;
        int j = 0;
        int hasOp = 0;
        while(j < 10000){

            int now = clock()/CLOCKS_PER_SEC;
            //cout << "static test: " << test << endl;
            if(clientlog.empty() == false && serverlog.empty() == false && hasOp == 0){
                hasOp = 1;
                cout << "static client log: " << clientlog << endl;
                cout << "static server log: " << serverlog << endl;
                clientlog = "";
                serverlog = "";
            }

//            if(now - lastTime > 0.5){
//                //cout << "static test: " << test << endl;
//                lastTime = now;
//            }

            j++;
        }

    }

private:
    static int test;
    static string serverlog;
    static string clientlog;
};



#endif //IPERFOUTPUTDATAANALYSIS_GDATA_H
