//
// Created by root on 18-4-8.
//

#include "SyncQueue.h"

SyncQueue::SyncQueue(int maxSize) : m_maxSize(maxSize), m_needStop(false) {

}

template<typename T>
void SyncQueue::Put(const T& x){
    Add(x);
}

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
template<typename T>
void SyncQueue::Take(T &list) {
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

template<typename T>
void SyncQueue::Take(T& t){
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
 *
 */
void SyncQueue::Stop(){

    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_needStop = true;
    }
    m_notFull.notify_all();
    m_notEmpty.notify_all();
}


