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
    //template<typename T>
    void Put(const T& x){
        Add(x);
    }
    void Put(T&& x);
    /**
     * 先创建一个unique获取mutex，然后再通过条件变量m_notEmpty来等待判断式，判断式由两个条件组成，
     * 一个是停止的标志，另一个是不为空的条件，当不满足任何一个条件时，条件变量会释放mutex并将线程至于
     * waiting状态，等待其他线程调用notify_one/notify_all将其唤醒；当满足任何一个条件时，则继续往
     * 下执行后面的逻辑，即将队列中的任务取出，并唤醒一个正处于等待状态的添加任务的线程取添加任务。当
     * 处于waiting状态的线程被notify_one/notify_all唤醒时，条件变量会重新获取mutex，然后再检查条件
     * 是否满足，如果满足，则往下执行，如果不满足，则释放mutext继续等待。
     * @tparam T
     * @param list
     */
    //template<typename T>
    void Take(std::list<T> &list) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{
            return m_needStop || NotEmpty();
        });
        if(m_needStop){
            return;
        }
        list = std::move(m_queue);
        m_notFull.notify_one();
    }
    //template<typename T>
    void Take(T& t){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{
            return m_needStop || NotEmpty();
        });
        if(m_needStop){
            return;
        }
        t = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }


    /**
     *stop函数先获取mutex，然后将停止标志设置为true。注意，这里为了保证线程安全，这里需要先获取mutex，
     * 在将其标志置为true之后，再唤醒所有等待的线程，因为等待的条件是m_needStop，并且满足条件，所以线程
     * 会继续往下执行。由于线程在m_needStop为true时会退出,所以所有的等待线程会相继退出。另外，把m_notFull
     * .notify_all()放到lock_guard保护范围之外了，这里页可以将m_notFull.notify_all()放到lock_guard
     * 保护范围之内，放到外面是为了一点优化。因为notify_one或notify_all会唤醒一个在等待的线程，线程被唤醒后
     * 会先获取mutex再检查条件是否满足，如果这时被lock_guard保护，被唤醒的线程需要lock_guard析构释放mutex
     * 才能获取。如果在lock_guard之外nortify_one或nortify_all,被唤醒的线程获取锁的时候不需要等待lock_guard
     * 释放锁，性能会好一点，所以在执行notify_one/notify_all时不需要枷锁保护
     */

    void Stop(){

        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_needStop = true;
        }
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    bool Empty();
    bool Full();
    size_t Size();
    int Count();

private:
    bool NotFull() const;
    bool NotEmpty() const;

    /**
     * Add的过程和Take的过程类似，也是先获取mutex,然后检查条件是否满足，不满足条件时，释放mutex继续等待，
     * 如果条件满足，则将新的任务插入到队列中，并唤醒取任务的线程去取数据。
     * @tparam F
     * @param x
     */
    template<typename F>
    void Add(F&& x){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this]{
            return m_needStop || NotFull();
        });
        if(m_needStop){
            return;
        }
        m_queue.push_back(std::forward<F>(x));
        m_notEmpty.notify_one();
    }




};


#endif //IPERFOUTPUTDATAANALYSIS_SYNCQUEUE_H
