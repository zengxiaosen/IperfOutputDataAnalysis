#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <dirent.h>
#include <queue>
#include <cstring>
#include <map>
#include <sstream>
#include <iterator>
#include <thread>
#include "Tools.h"

#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <future>
#include "zbInfoNode.h"
#include "ThreadPool.hpp"
#include "TaskFunctionCollections.hpp"
#include "PreIO.hpp"


using namespace std;

int main() {

    /**
     * 读取文件信息
     */

    /**
     * 说明：
     * 版本一是用mininet产出的数据做带宽相关的统计
     * 但目前版本此部分功能已经改成用控制器onos来统计了
     * 同时，统计mininet产出的log无法统计link级别的带宽（bps）
     * 只能统计流级别的带宽（bps）
     * 所以带宽方面的统计全部移交控制器
     */
    cout << "统计iperf产出的流量日志信息" << std::endl;

    queue<string> q_server ;
    queue<string> q_client;
    PreIO preIO("/home/zengxiaosen/log");
    preIO.WriteDataToQueue(q_server, q_client);

    /**
     * 线程池初始创建两个线程
     * 线程池内的线程会并行处理同步队列中的任务
     */
    ThreadPool pool;

    TaskFunctionCollections* taskFunctionCollections = TaskFunctionCollections::getInstance();
    pthread_mutex_init(&TaskFunctionCollections::mtx, 0);
    std::thread thd1([&pool, &q_client, &taskFunctionCollections]{
        auto thdId = this_thread::get_id();

        pool.AddTask([thdId, q_client, taskFunctionCollections]{
            cout << "分发解析client日志任务,同步层线程1的线程id： " << thdId << endl;
            taskFunctionCollections->function2(q_client);
        });
    });

    std::thread thd2([&pool, &q_server, &taskFunctionCollections]{
        auto thdId = this_thread::get_id();
        pool.AddTask([thdId, q_server, taskFunctionCollections]{
            cout << "分发解析server日志任务,同步层线程2的线程id： " << thdId << endl;
            taskFunctionCollections->function1(q_server);
        });
    });

    this_thread::sleep_for(std::chrono::seconds(2));
    cout << "请输入任意字符结束: " << endl;
    getchar();
    pool.Stop();
    thd1.join();
    thd2.join();




    return 0;
}




