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
#include "Tools.h"
#include "zbInfoNode.h"

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
    DIR *directory_pointer;
    struct dirent *entry;
    queue<string> q_server ;
    queue<string> q_client;
    if((directory_pointer=opendir("/home/zengxiaosen/log")) == NULL){
        cout << "Error open\n" << endl;
        return 2;
    }else{
        while((entry = readdir(directory_pointer)) != NULL){
            if(entry->d_name[0] == '.')
                continue;
            //cout << entry->d_name << endl;
            string s = entry->d_name;
            if(strstr(s.c_str(), "delay") == NULL){
                //非delay
                if(strstr(s.c_str(), "client") == NULL){
                    //不存在client字符串
                    q_server.push(s);
                } else{
                    q_client.push(s);
                }
            }
        }
    }
    map<string, zbInfoNode> index_infoServerNode;

    Tools tools;
    tools.readQueue(q_server);
    tools.readQueueClient(q_client);





    return 0;
}