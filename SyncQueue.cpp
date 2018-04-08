//
// Created by root on 18-4-8.
//
#include "SyncQueue.hpp"
#include <thread>


//using namespace std;
template<typename T>
SyncQueue<T>::SyncQueue(int maxSize) : m_maxSize(maxSize), m_needStop(false) {

}






template<typename T>
bool SyncQueue<T>::Empty(){
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_queue.empty();
}

template<typename T>
bool SyncQueue<T>::Full(){
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_queue.size() == m_maxSize;
}

template<typename T>
size_t SyncQueue<T>::Size(){
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_queue.size();
}

template<typename T>
int SyncQueue<T>::Count(){
    return m_queue.size();
}

template<typename T>
bool SyncQueue<T>::NotFull() const{
    bool full = m_queue.size() >= m_maxSize;
    if(full){
        cout << "缓冲区满了，需要等待..." << endl;
    }
    return !full;
}

template<typename T>
bool SyncQueue<T>::NotEmpty() const {
    bool empty = m_queue.empty();
    if(empty){
        cout << "缓冲区空了，需要等待..., 异步层的线程ID： " << this_thread::get_id() << endl;
    }
    return !empty;
}



