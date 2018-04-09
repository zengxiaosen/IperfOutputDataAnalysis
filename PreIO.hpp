//
// Created by root on 18-4-9.
//

#ifndef IPERFOUTPUTDATAANALYSIS_PREIO_H
#define IPERFOUTPUTDATAANALYSIS_PREIO_H
#include <string>
#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <queue>
#include <cstring>
using namespace std;
class PreIO{
public:
    PreIO(){

    }
    PreIO(string s){
        this->directory = s;
    }
    void WriteDataToQueue(queue<string>& q_server, queue<string>& q_client){
        DIR *directory_pointer;
        struct dirent *entry;
        //"/home/zengxiaosen/log"
        if((directory_pointer=opendir(directory.c_str())) == NULL){
            cout << "Error open\n" << endl;
        }else{
            while((entry = readdir(directory_pointer)) != NULL){
                if(entry->d_name[0] == '.')
                    continue;
                //cout << entry->d_name << endl;
                string s = entry->d_name;
                //cout << s << endl;
                // strstr(s.c_str(), "delay") == NULL
                string::size_type idx = s.find("delay");
                if(idx != string::npos){
                    //字符串含有delay字串
                    //不处理

                }else{
                    //字符串不含字串
                    idx = s.find("c");
                    if(idx != string::npos){
                        //字符串含有client子串
                        q_client.push(s);
                        //cout << s << endl;
                    }else{
                        //字符串不含client字串
                        //server端

                        q_server.push(s);
                    }
                }

            }
        }


    }

private:
    string directory;


};

#endif //IPERFOUTPUTDATAANALYSIS_PREIO_H
