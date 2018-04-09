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
#include "Aspect.hpp"
#include "LoggingAspect.hpp"
#include "TimeElapsedAspect.hpp"
#include "MainWork.hpp"
#include "Ioc.hpp"


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

    queue<string> q_server;
    queue<string> q_client;
    PreIO preIO("/home/zengxiaosen/log");
    preIO.WriteDataToQueue(q_server, q_client);

    /**
     * 测试：通过aop记录目标函数的执行时间并输出日志，
     * 其中时间和日志放到切面中。
     * 在执行函数之前输出日志，在执行完成之后也输出日志，
     * 并对执行的函数进行计时。
     */
    //Invoke<LoggingAspect, TimeElapsedAspect>(&foo, 1); //织入方法
    /**
     * 线程池初始创建两个线程
     * 线程池内的线程会并行处理同步队列中的任务
     */
    //织入方法
    Invoke<LoggingAspect, TimeElapsedAspect>(&MainWork::DealStatistic, q_server, q_client);
    //MainWork::DealStatistic(q_server, q_client);




    return 0;
}






