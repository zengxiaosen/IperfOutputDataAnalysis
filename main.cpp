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

    void toString(){
        cout << "bandwidth: " << bandwidth << ", packetloss: " << losspackets << ", allpackets: " << allpackets << endl;
    }

private:
    //丢包数
    string allpackets; //总包数
};

map<string, zbInfoNode>  ReadDataFromFileLBLIntoCharArray(string s)
{

    map<string, zbInfoNode> map;
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
            map.insert(pair<string, zbInfoNode>(p, zb));
            //cout << "map.size() = " << map.size() << endl;
        }else{
            zb.setSrcIp(srcIP);
            zb.setDstIp(dstIP);
            zb.setIndex(p);
//            for(auto s: strs){
//                cout << s << endl;
//            }
            //cout << strs[7] << endl;
            zb.setBandwidth(strs[7]);
            //cout << strs[12] << endl;
            zb.setAllpackets(strs[12]);
            //cout << strs[11] << endl;
            zb.setLosspackets(strtok((char *) strs[11].c_str(), "/"));
            map.insert(pair<string, zbInfoNode>(p, zb));
        }



        j++;
    }

    //cout << map.size() << endl;
    return map;

}

void readQueue(queue<string> &queue) {
    int n = queue.size();
    string e;
    /**
     * key index
     * value 第index秒的平均带宽
     */
    map<string, int> index_bandwitdh;
    /**
     * key index
     * value 第index秒的总丢包数
     */
    map<string, int> index_droppackets;
    /**
     * key index
     * value 第index秒的总传输包数
     */
    map<string, int> index_allpackets;


    for(int i=0; i< n; i++){
        e = queue.front();
        string frontstring = "/home/zengxiaosen/log/";
        if(e.find("client") == -1){
            //没找到，即为server类型
            map<string, zbInfoNode> map = ReadDataFromFileLBLIntoCharArray(frontstring+e);
            /**
             * map.size() = 51 而不是 60 待修复
             * status: 已修复
             */

            //cout << "map.size()" << map.size() << endl;

            /**
             * key:第几秒
             * value：zbinfoNode
             */
            for(const pair<string, zbInfoNode> & maplocal : map){
                string key = maplocal.first;
                zbInfoNode value = maplocal.second;
//                cout << "key: " << key << endl;
//                cout << "value: " << endl;
                /**
                 * 这里注意，10以下的key解析出现字段错位
                 *  key: 6.0
                    value:
                    bandwidth: KBytes, packetloss: ms, allpackets: 0/
                    待修复

                    status:已修复
                 */
//                value.toString();

                /**
                 * 对bandwith进行秒级别的聚合
                 */
                if(index_bandwitdh[key] == 0 || index_bandwitdh[key] == NULL){
                    index_bandwitdh[key] = 0;
                }
                int base_bandwith = index_bandwitdh[key];
                //cout << base_bandwith << endl;
                int add_bandwith = atoi(value.getBandwidth().c_str());
                //cout << add_bandwith << endl;
                int tempsum_bandwith = base_bandwith + add_bandwith;

                index_bandwitdh[key] = tempsum_bandwith;
                //cout << index_bandwitdh[key] << endl;


                /**
                 * 对droppackets进行秒级别聚合
                 */
                if(index_droppackets[key] == 0 || index_droppackets[key] == NULL){
                    index_droppackets[key] = 0;
                }
                int base_droppackets = index_droppackets[key];
                int add_droppackets = atoi(value.getLosspackets().c_str());
                int tempsum_droppackets = base_droppackets + add_droppackets;
                index_droppackets[key] = tempsum_droppackets;
                //cout << index_droppackets[key] << endl;


                /**
                 * 对allpackets进行秒级别的聚合
                 */
                if(index_allpackets[key] == 0 || index_allpackets[key] == NULL){
                    index_allpackets[key] = 0;
                }
                int base_allpackets = index_allpackets[key];
                int add_allpackets = atoi(value.getAllpackets().c_str());
                int tempsum_allpackets = base_allpackets + add_allpackets;
                index_allpackets[key] = tempsum_allpackets;
                //cout << index_allpackets[key] << endl;




            }
            cout << "==========" << endl;


            queue.pop();
        }else{
            //找到client标识，即为client类型
            //queue.pop();
            /**
             * client类型的node放到别的方法,这块废弃
             */
        }


    }


    /**
     * test 三个统计的hashmap
     * 得到1分钟内每秒的所有流的带宽信息，丢包信息，总包数信息(16条)
     */
    cout << "index_bandwitdh.size(): " << index_bandwitdh.size() << endl;
    cout << "index_droppackets.size(): " << index_droppackets.size() << endl;
    cout << "index_allpackets.size(): " << index_allpackets.size() << endl;

    /**
     * 写到数据库
     * springboot或django展示
     */

    auto index_bandwith_begin = index_bandwitdh.begin();
    while(index_bandwith_begin != index_bandwitdh.end()){

        string key = index_bandwith_begin->first;
        int value = index_bandwith_begin->second;
        double meanvalue = value / 16; //目前设置的是16条流
        /**
         * key value写入数据库
         * key 1-60(0-59)
         * value 每秒 16条流的平均值
         */


        //===========================================



        index_bandwith_begin++;
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

    /**
     * 说明：
     * 注释调的代码是用mininet产出的数据做带宽相关的统计
     * 但目前版本此部分功能已经改成用控制器onos来统计了
     * 同时，统计mininet产出的log无法统计link级别的带宽（bps）
     * 只能统计流级别的带宽（bps）
     * 所以带宽方面的统计全部移交控制器
     */
//    DIR *directory_pointer;
//    struct dirent *entry;
//    queue<string> q_server;
//    queue<string> q_client;
//    if((directory_pointer=opendir("/home/zengxiaosen/log")) == NULL){
//        cout << "Error open\n" << endl;
//        return 2;
//    }else{
//        while((entry = readdir(directory_pointer)) != NULL){
//            if(entry->d_name[0] == '.')
//                continue;
//            //cout << entry->d_name << endl;
//            string s = entry->d_name;
//            if(strstr(s.c_str(), "delay") == NULL){
//                //非delay
//                if(strstr(s.c_str(), "client") == NULL){
//                    //不存在client字符串
//                    q_server.push(s);
//                } else{
//                    q_client.push(s);
//                }
//            }
//        }
//    }
//    map<string, zbInfoNode> index_infoServerNode;
//    readQueue(q_server);
// readQueueClient(q_client);

// ===================================================





    return 0;
}