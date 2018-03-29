//
// Created by root on 18-3-18.
//

#include <iostream>
#include <map>
#include <cstring>
#include "Tools.h"
#include "zbInfoNode.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

double Tools::ReadClientDataFromFileToDouble(string s) {
    //cout << s << endl;
    double result = 0;
    ifstream fin(s);
    const   int  LINE_LENGTH =  100 ;
    char  str[LINE_LENGTH];
    int i = 0;
    //过滤前7行
    while( fin.getline(str, LINE_LENGTH) && i < 9){
        i++;
    }
    //到达有信息的行
    while( fin.getline(str, LINE_LENGTH) && i < 10){
        i++;
        istringstream iss(str);
        vector<string> strs{istream_iterator<string>(iss), istream_iterator<string>()};
//        for(string s : strs){
//            cout << s << endl;
//        }

        // strs[11] : (0%)
        string Pertemp = strs[strs.size()-1];
        string strs11_etl = Pertemp.substr(1, Pertemp.size()-2);
        // strs11_etl : 0.012%
        double packetls = atof(strs11_etl.substr(0, strs11_etl.size()-1).c_str());
        //packetls : 0.012
        //cout << packetls << " " << endl;
        result = packetls;
    }
    return result;

}

void Tools::readQueueClient(queue<string> q) {
    int n = q.size();
    string e;
    map<string, string> file_packetloss;
    double sum = 0;
    for(int i=0; i< n; i++){
        e = q.front();
        string frontstring = "/home/zengxiaosen/log/";
        if(e.find("client") != -1){
            //frontstring+e 就是要读的文件名
            sum += ReadClientDataFromFileToDouble(frontstring+e);
        }
        q.pop();
    }
    //平均每個（src，dst）的丢包率
    cout << "c平均丢包率" << sum / n << "%" << endl;
    double meanPL = sum / n;
    //每次测试，比如iperf 300秒，会产生一个值
    //第1行，代表源目打3M， link capacity为 10M
    //第2行，代表源目打4M， link capacity为 10M
    //以此类推，第 9行，代表源目打9M， link capacity为 10M
    string filePath = "/home/zengxiaosen/packetLoss.log";
    ConcatToFile(meanPL, filePath);
}

void Tools::ConcatToFile(double pl, string basic_string) {
    //追加写
}

map<double, zbInfoNode, less<double>>  Tools::ReadDataFromFileLBLIntoCharArray(string s)
{
    cout << s << endl;
    map<double, zbInfoNode, less<double>> map;
    ifstream fin(s);
    const   int  LINE_LENGTH =  100 ;
    char  str[LINE_LENGTH];
    int i = 0;
    //过滤前7行
    while( fin.getline(str, LINE_LENGTH) && i < 4){
        i++;
    }

    string srcIP = "";
    string dstIP = "";
    while( fin.getline(str, LINE_LENGTH) && i < 5){
        i++;
        //cout << str << endl;
        //[  3] local 10.0.0.9 port 5001 connected with 10.0.0.12 port 44127
        istringstream iss_ip(str);
        vector<string> strs{istream_iterator<string>(iss_ip), istream_iterator<string>()};

        //client_ip
        //cout << strs[3] << endl;
        //server_ip
        //cout << strs[8] << endl;
        srcIP = strs[3];
        dstIP = strs[8];


    }


    //取60秒的数据做实验
    int j=0;
    //=============
    while ( fin.getline(str,LINE_LENGTH) && j < 60)
    {
        //cout << str << endl;
        /**
         * 对每行信息进行etl处理
         * index：j（第几秒） 0代表0~1
         * bandwidth
         * 丢包率
         */
        //cout << j << endl;
        istringstream iss(str);
        vector<string> strs{istream_iterator<string>(iss), istream_iterator<string>()};


        //example: 59.0-60.0
        //cout << strs[2] << endl;

        //带宽：rate:47.0 Kbits/sec
        //example:47.0
        //cout << strs[6] << endl;

        //丢包数
        //example:0
        //cout << strs[10].substr(0, strs[10].length()-1) << endl;
        //这秒的总包数
        //example:4
        //cout << strs[11] << endl;
        char* p = strtok((char *) strs[2].c_str(), "-");

        string temp_p = p;

        int int_p = atoi(temp_p.c_str());
        //cout << int_p << endl;
        zbInfoNode zb;
        if(int_p >= 9){
            zb.setSrcIp(srcIP);
            zb.setDstIp(dstIP);

            zb.setBandwidth(strs[6]);
            zb.setIndex(p);
            zb.setAllpackets(strs[11]);
            zb.setLosspackets(strtok((char *) strs[10].c_str(), "/"));

            //cout << strtok((char *) strs[10].c_str(), "/") << endl;
            //cout << strs[2] << endl;

            //cout << p << endl;

            double p_d = atof(p);
            map.insert(pair<double, zbInfoNode>(p_d, zb));
            //cout << "map.size() = " << map.size() << endl;
        }else{
            zb.setSrcIp(srcIP);
            zb.setDstIp(dstIP);
            zb.setIndex(p);
//            for(auto s: strs){
//                cout << s << endl;
//            }
            //cout << strs[7] << endl;
            //这个值没意义，是打流的时候的流速...
            zb.setBandwidth(strs[7]);
            //cout << strs[12] << endl;
            if (strs.size() >= 13)
                zb.setAllpackets(strs[12]);
            else
                zb.setAllpackets("0");
            //cout << strs[11] << endl;
            if (strs.size() >= 12)
                zb.setLosspackets(strtok((char *) strs[11].c_str(), "/"));
            else
                zb.setLosspackets("0");
            double p_d = atof(p);
            map.insert(pair<double, zbInfoNode>(p_d, zb));
        }



        j++;
    }
    //==================


    //cout << map.size() << endl;
    return map;

}

void Tools::readQueue(queue<string> &q) {
    int n = q.size();
    string e;
    map<string, string> file_packetloss;
    double sum = 0;
    for(int i=0; i< n; i++){
        e = q.front();
        string frontstring = "/home/zengxiaosen/log/";
        if(e.find("client") == -1){
            //frontstring+e 就是要读的文件名
            sum += ReadServerDataFromFileToDouble(frontstring+e);
        }
        q.pop();
    }
    //平均每個（src，dst）的丢包率
    cout << "平均丢包率" << sum / n << "%" << endl;
    double meanPL = sum / n;

}


double Tools::ReadServerDataFromFileToDouble(string s) {
    //cout << s << endl;
    double result = 0;
    ifstream fin(s);
    const   int  LINE_LENGTH =  100 ;
    char  str[LINE_LENGTH];
    int index = 0;

    while( fin.getline(str, LINE_LENGTH)){
        index++;
    }
    //cout << index << "===" << endl;
    int i = 0;
    fin.close();
    ifstream fin1(s);
    //到达有信息的行
    while( fin1.getline(str, LINE_LENGTH) && i < index){
        i++;
        if(i == index-1){
            istringstream iss(str);
            vector<string> strs{istream_iterator<string>(iss), istream_iterator<string>()};
//            for(string s : strs){
//                cout << s << endl;
//            }

            // strs[11] : (0%)
            string Pertemp = strs[strs.size()-1];
            string strs11_etl = Pertemp.substr(1, Pertemp.size()-2);
            // strs11_etl : 0.012%
            double packetls = atof(strs11_etl.substr(0, strs11_etl.size()-1).c_str());
            //packetls : 0.012
            //cout << packetls << " " << endl;
            result = packetls;
        }
    }
    return result;

}

