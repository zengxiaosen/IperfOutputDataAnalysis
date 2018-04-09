//
// Created by root on 18-4-9.
//
#include <iostream>
#include "Timer.hpp"

#ifndef IPERFOUTPUTDATAANALYSIS_TIMEELAPSEDASPECT_H
#define IPERFOUTPUTDATAANALYSIS_TIMEELAPSEDASPECT_H
using namespace std;
struct TimeElapsedAspect{
    void Before(queue<string> q_server, queue<string> client){
        m_lastTime = m_t.elapsed();
    }

    void After(queue<string> q_server, queue<string> client){
        cout << "time elapsed: " << m_t.elapsed() - m_lastTime << endl;
    }

private:
    double m_lastTime;
    Timer m_t;
};
#endif //IPERFOUTPUTDATAANALYSIS_TIMEELAPSEDASPECT_H
