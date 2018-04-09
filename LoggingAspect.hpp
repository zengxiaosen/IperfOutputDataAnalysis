//
// Created by root on 18-4-9.
//

#ifndef IPERFOUTPUTDATAANALYSIS_LOGGINGASPECT_H
#define IPERFOUTPUTDATAANALYSIS_LOGGINGASPECT_H

#include <iostream>

struct LoggingAspect
{
    void Before(queue<string> q_server, queue<string> client)
    {
        std::cout <<"entering"<< std::endl;
    }
    void After(queue<string> q_server, queue<string> client)
    {
        std::cout <<"leaving"<< std::endl;
    }
};
#endif //IPERFOUTPUTDATAANALYSIS_LOGGINGASPECT_H
