//
// Created by root on 18-3-18.
//
#include <iostream>
#ifndef IPERFOUTPUTDATAANALYSIS_ZBINFONODE_H
#define IPERFOUTPUTDATAANALYSIS_ZBINFONODE_H
using namespace std;

class zbInfoNode {

private:

    string srcIp;
    string dstIp;
    string index; // 第几秒
    string bandwidth;
public:

    void toString() const;

    const string &getSrcIp() const;

    void setSrcIp(const string &srcIp);

    const string &getDstIp() const;

    void setDstIp(const string &dstIp);

    const string &getIndex() const;

    void setIndex(const string &index);

    const string &getBandwidth() const;

    void setBandwidth(const string &bandwidth);

    const string &getLosspackets() const;

    void setLosspackets(const string &losspackets);

    const string &getAllpackets() const;

    void setAllpackets(const string &allpackets);

private:
//带宽
    string losspackets;
    string allpackets; //总包数

public:

};


#endif //IPERFOUTPUTDATAANALYSIS_ZBINFONODE_H
