//
// Created by root on 18-4-9.
//

#ifndef IPERFOUTPUTDATAANALYSIS_MAINWORK_H
#define IPERFOUTPUTDATAANALYSIS_MAINWORK_H
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include "ThreadPool.hpp"
#include <queue>
#include "TaskFunctionCollections.hpp"
class MainWork{
public:
    static void DealStatistic(queue<string> q_server, queue<string> q_client){
        ThreadPool pool;

        TaskFunctionCollections* taskFunctionCollections = TaskFunctionCollections::getInstance();
        pthread_mutex_init(&TaskFunctionCollections::mtx, 0);
        std::thread thd1([&pool, &q_client, &taskFunctionCollections]{
            auto thdId = this_thread::get_id();

            pool.AddTask([thdId, q_client, taskFunctionCollections]{
                cout << "分发解析client日志任务,同步层线程1的线程id： " << thdId << endl;
                taskFunctionCollections->function2(q_client);
                //Invoke<LoggingAspect, TimeElapsedAspect>(taskFunctionCollections->function2, q_client);
            });
        });
        cout << "----" << endl;
        std::thread thd2([&pool, &q_server, &taskFunctionCollections]{
            auto thdId = this_thread::get_id();
            pool.AddTask([thdId, q_server, taskFunctionCollections]{
                cout << "分发解析server日志任务,同步层线程2的线程id： " << thdId << endl;
                taskFunctionCollections->function1(q_server);
                //Invoke<LoggingAspect, TimeElapsedAspect>(taskFunctionCollections->function1, q_server);
            });
        });

        this_thread::sleep_for(std::chrono::seconds(2));
        cout << "请输入任意字符结束: " << endl;
        getchar();
        pool.Stop();
        thd1.join();
        thd2.join();
    }
};
#endif //IPERFOUTPUTDATAANALYSIS_MAINWORK_H
