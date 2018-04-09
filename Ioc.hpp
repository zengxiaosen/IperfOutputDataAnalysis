//
// Created by root on 18-4-9.
//
/**
 * 通过Ioc容器来消除对象直接依赖产生的耦合性。
 * 具体做法是将对象的依赖关系从代码中移出去，
 * 放在一个统一的XML配置文件中或者在Ioc容器中
 * 配置这种依赖关系，由Ioc容器来管理对象的依赖关系。
 *
 * 实现Ioc容器需要解决三个问题，
 * 1,创建所有类型的对象
 * 2,类型擦除
 * 3,创建依赖对象
 */
#ifndef IPERFOUTPUTDATAANALYSIS_IOC_H
#define IPERFOUTPUTDATAANALYSIS_IOC_H

#include <string>
#include <map>
#include <memory>
#include <functional>
using namespace std;
//一个简单的可配值的对象工厂
template <class T>
class IocContainer_v1{
public:
    IocContainer_v1(void){

    }
    ~IocContainer_v1(void){

    }
    //注册需要创建对象的构造函数，需要传入一个唯一的标识，以便在后面创建对象时方便查找
    template<class Drived>
    void RegisterType(string strKey){

    }

};
#endif //IPERFOUTPUTDATAANALYSIS_IOC_H
