//
// Created by root on 18-4-9.
//

#ifndef IPERFOUTPUTDATAANALYSIS_TIMER_H
#define IPERFOUTPUTDATAANALYSIS_TIMER_H
#include<chrono>
using namespace std;
using namespace std::chrono;
class Timer{
private:
    time_point<high_resolution_clock> m_begin;
public:
    Timer() : m_begin(high_resolution_clock::now()){

    }
    //默认输出毫秒
    int64_t elapsed() const{
        return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - m_begin).count();
    }
    //默认输出秒
    double elapsed_second() const
    {
        return duration_cast<duration<double>>(high_resolution_clock::now() - m_begin).count();
    }

    //微秒
    int64_t elapsed_micro() const
    {
        return duration_cast<chrono::microseconds>(high_resolution_clock::now() - m_begin).count();
    }

    //纳秒
    int64_t elapsed_nano() const
    {
        return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - m_begin).count();
    }

    ////秒
    //int64_t elapsed_seconds() const
    //{
    //	return duration_cast<chrono::seconds>(high_resolution_clock::now() - m_begin).count();
    //}

    //分
    int64_t elapsed_minutes() const
    {
        return duration_cast<chrono::minutes>(high_resolution_clock::now() - m_begin).count();
    }

    //时
    int64_t elapsed_hours() const
    {
        return duration_cast<chrono::hours>(high_resolution_clock::now() - m_begin).count();
    }
};
#endif //IPERFOUTPUTDATAANALYSIS_TIMER_H
