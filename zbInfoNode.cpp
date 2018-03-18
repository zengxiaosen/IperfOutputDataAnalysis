//
// Created by root on 18-3-18.
//

#include "zbInfoNode.h"

const string &zbInfoNode::getSrcIp() const {
    return srcIp;
}

void zbInfoNode::setSrcIp(const string &srcIp) {
    zbInfoNode::srcIp = srcIp;
}

const string &zbInfoNode::getDstIp() const {
    return dstIp;
}

void zbInfoNode::setDstIp(const string &dstIp) {
    zbInfoNode::dstIp = dstIp;
}

const string &zbInfoNode::getIndex() const {
    return index;
}

void zbInfoNode::setIndex(const string &index) {
    zbInfoNode::index = index;
}

const string &zbInfoNode::getBandwidth() const {
    return bandwidth;
}

void zbInfoNode::setBandwidth(const string &bandwidth) {
    zbInfoNode::bandwidth = bandwidth;
}

const string &zbInfoNode::getLosspackets() const {
    return losspackets;
}

void zbInfoNode::setLosspackets(const string &losspackets) {
    zbInfoNode::losspackets = losspackets;
}

const string &zbInfoNode::getAllpackets() const {
    return allpackets;
}

void zbInfoNode::setAllpackets(const string &allpackets) {
    zbInfoNode::allpackets = allpackets;
}

void zbInfoNode::toString() const {
    cout << "bandwidth: " << zbInfoNode::bandwidth << ", packetloss: " << zbInfoNode::losspackets << ", allpackets: " << zbInfoNode::allpackets << endl;
}