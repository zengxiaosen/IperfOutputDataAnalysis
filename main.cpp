#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <dirent.h>
#include <queue>
#include <cstring>

using namespace std;


int main() {

    /**
     * 读取文件信息
     */
    cout << "统计iperf产出的流量日志信息" << std::endl;
    DIR *directory_pointer;
    struct dirent *entry;
    queue<string> q_server;
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

    cout << "q_server.size() : " << q_server.size() << endl;
    cout << "q_client.size() : " << q_client.size() << endl;


    return 0;
}