//
// Created by root on 18-4-8.
//
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <list>
using namespace std;
#ifndef IPERFOUTPUTDATAANALYSIS_SYNCQUEUE_H
#define IPERFOUTPUTDATAANALYSIS_SYNCQUEUE_H
/**
 * 同步队列
 * @tparam T
 */
template<typename T>
class SyncQueue {
private:
    std::list<T> m_queue;//缓冲区
    std::mutex m_mutex;//互斥量和条件变量结合起来使用
    std::condition_variable m_notEmpty;//不为空的条件变量
    std::condition_variable m_notFull;//没有满的条件变量
    int m_maxSize;//同步队列最大的size
    bool m_needStop;//停止的标记
public:
    SyncQueue(int maxSize);
    void Put(const T&x);
    void Put(T&& x);
    void Take(std::list<T>& list);
    void Take(T& t);
    void Stop();
    bool Empty();
    bool Full();
    size_t Size();
    int Count();

private:
    bool NotFull() const;
    bool NotEmpty() const;
    template<typename F>
    void Add(F&& x);




};


#endif //IPERFOUTPUTDATAANALYSIS_SYNCQUEUE_H
