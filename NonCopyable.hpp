//
// Created by root on 18-4-9.
//
/**
 * 如果希望一个类不可复制，要使类成为不可复制，典型的实现方法是将类的
 * 复制构造函数和赋值运算符设置为private或protected。如果两者都未
 * 定义，那么编译器会提供一一种作为公共成员函数的隐式版本。C++11提供
 * 了Default和Delete函数，使我们可以更方便地实现一个NonCopyable了，
 * 如果希望类不被复制，则直接从这个NonCopyable派生即可。
 */
#ifndef IPERFOUTPUTDATAANALYSIS_NONCOPYABLE_H
#define IPERFOUTPUTDATAANALYSIS_NONCOPYABLE_H

class NonCopyable{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;//禁用复制构造
    NonCopyable& operator = (const NonCopyable&) = delete;//禁用赋值构造
};
#endif //IPERFOUTPUTDATAANALYSIS_NONCOPYABLE_H
