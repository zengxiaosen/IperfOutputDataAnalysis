//
// Created by root on 18-4-9.
//

#ifndef IPERFOUTPUTDATAANALYSIS_TASKFUNCTIONCOLLECTIONS_H
#define IPERFOUTPUTDATAANALYSIS_TASKFUNCTIONCOLLECTIONS_H

#include "Tools.h"
#include "LogData.hpp"
#include <queue>
/**
 * 这个类相当于一个任务管理器
 * 使用单例模式
 * 访问量不大时使用懒汉模式
 * 访问量大时可以改成饿汉模式
 */
class TaskFunctionCollections{

private:
    TaskFunctionCollections(){
        pthread_mutex_init(&mtx, 0);
    }
    static TaskFunctionCollections *singleTask;
public:
    static pthread_mutex_t mtx;
    static TaskFunctionCollections* getInstance(){
        if(singleTask == NULL){
            pthread_mutex_lock(&mtx);
            if(singleTask == NULL){
                singleTask = new TaskFunctionCollections();
            }
            pthread_mutex_lock(&mtx);
        }
        return singleTask;
    }
    void function2(queue<string> queue) {

        cout << "取任务执行的异步层线程id： " << endl;
        cout  << std::this_thread::get_id() << endl;
        Tools tools;
        tools.readQueueClient(queue);

    }
    void function1(queue<string> queue) {
        cout << "取任务执行的异步层线程id： " << endl;
        cout  << std::this_thread::get_id() << endl;
        Tools tools;
        tools.readQueue(queue);
    }

};
pthread_mutex_t TaskFunctionCollections::mtx;
TaskFunctionCollections* TaskFunctionCollections::singleTask=NULL;


#endif //IPERFOUTPUTDATAANALYSIS_TASKFUNCTIONCOLLECTIONS_H
