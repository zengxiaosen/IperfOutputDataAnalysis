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

using namespace std;

class zbInfoNode{



private:

    string srcIp;
    string dstIp;
    string index; // 第几秒
    string bandwidth;//带宽
    string losspackets;
public:
    const string &getSrcIp() const {
        return srcIp;
    }

    void setSrcIp(const string &srcIp) {
        zbInfoNode::srcIp = srcIp;
    }

    const string &getDstIp() const {
        return dstIp;
    }

    void setDstIp(const string &dstIp) {
        zbInfoNode::dstIp = dstIp;
    }

    const string &getIndex() const {
        return index;
    }

    void setIndex(const string &index) {
        zbInfoNode::index = index;
    }

    const string &getBandwidth() const {
        return bandwidth;
    }

    void setBandwidth(const string &bandwidth) {
        zbInfoNode::bandwidth = bandwidth;
    }

    const string &getLosspackets() const {
        return losspackets;
    }

    void setLosspackets(const string &losspackets) {
        zbInfoNode::losspackets = losspackets;
    }

    const string &getAllpackets() const {
        return allpackets;
    }

    void setAllpackets(const string &allpackets) {
        zbInfoNode::allpackets = allpackets;
    }

private:
    //丢包数
    string allpackets; //总包数
};

void  ReadDataFromFileLBLIntoCharArray(string s, map<string, zbInfoNode>& map)
{

    ifstream fin(s);
    const   int  LINE_LENGTH =  100 ;
    char  str[LINE_LENGTH];
    int i = 0;
    //过滤前7行
    while( fin.getline(str, LINE_LENGTH) && i < 6){
        i++;
    }
    //取60秒的数据做实验
    int j=0;
    while ( fin.getline(str,LINE_LENGTH) && j < 60)
    {
        cout << str << endl;
        /**
         * 对每行信息进行etl处理
         * index：j（第几秒） 0代表0~1
         * bandwidth
         * 丢包率
         */
        cout << j << endl;
        istringstream iss(str);
        vector<string> strs{istream_iterator<string>(iss), istream_iterator<string>()};
        for(auto &s : strs){
            //cout << s << endl;
        }
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

        zbInfoNode zb;
        zb.setBandwidth(strs[6]);
        zb.setIndex(j+"");
        zb.setAllpackets(strs[11]);
        zb.setLosspackets(strs[10]);
        cout << s.substr(22, s.length()) << endl;




        j++;
    }

}

void readQueue(queue<string> &queue, map<string, zbInfoNode>& map) {
    int n = queue.size();
    string e;
    for(int i=0; i< n; i++){
        e = queue.front();
        string frontstring = "/home/zengxiaosen/log/";
        if(e.find("client") == -1){
            //没找到，即为server类型
            ReadDataFromFileLBLIntoCharArray(frontstring+e, map);
            queue.pop();
        }else{
            //找到client标识，即为client类型
            queue.pop();
        }


    }
    if(queue.empty()){
        cout << endl;

    }

}

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

    map<string, zbInfoNode> index_infoServerNode;

    cout << "q_server.size() : " << q_server.size() << endl;
    readQueue(q_server, index_infoServerNode);
    cout << "q_client.size() : " << q_client.size() << endl;
    readQueue(q_client, index_infoServerNode);


    cout << q_server.size() << endl;
    cout << q_client.size() << endl;





    return 0;
}