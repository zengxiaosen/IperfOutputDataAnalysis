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
#include<string>
#include<unordered_map>
#include<memory>
#include<functional>
#include <memory>
#include <functional>
#include "NonCopyable.hpp"
#include "Any.hpp"

using namespace std;
class IocContainer : NonCopyable
{
public:
    IocContainer(void){}
    ~IocContainer(void){}

    template<class T, typename Depend, typename... Args>
    void RegisterType(const string& strKey)
    {
        std::function<T* (Args...)> function = [](Args... args){ return new T(new Depend(args...)); };//通过闭包擦除了参数类型
        RegisterType(strKey, function);
    }

    template<class T, typename... Args>
    T* Resolve(const string& strKey, Args... args)
    {
        if (m_creatorMap.find(strKey) == m_creatorMap.end())
            return nullptr;

        Any resolver = m_creatorMap[strKey];
        std::function<T* (Args...)> function = resolver.AnyCast<std::function<T* (Args...)>>();

        return function(args...);
    }

    template<class T, typename... Args>
    std::shared_ptr<T> ResolveShared(const string& strKey, Args... args)
    {
        T* t = Resolve<T>(strKey, args...);

        return std::shared_ptr<T>(t);
    }

private:
    void RegisterType(const string& strKey, Any constructor)
    {
        if (m_creatorMap.find(strKey) != m_creatorMap.end())
            throw std::invalid_argument("this key has already exist!");

        //通过Any擦除了不同类型的构造器
        m_creatorMap.emplace(strKey, constructor);
    }

private:
    unordered_map<string, Any> m_creatorMap;
};
#endif //IPERFOUTPUTDATAANALYSIS_IOC_H
