//
// Created by root on 18-4-9.
//
/**
 * AOP(Aspect-Oriented Programming 面向方面编程），是OOP的一种有益补充。
 * 面向对象编程中的继承是一种从上而下的关系，不适合定义从左到右的横向关系，如果
 * 继承体系中的很多无关联的对象都有一些公共行为，这些公共行为可能分散在不同的组件，
 * 不同的对象中，通过继承方式提取这些公共行为不太合适。使用AOP还有一种情况是为了提高
 * 程序的可维护性，AOP将程序的非核心逻辑都“横切”出来，将非核心逻辑和核心逻辑分离，使
 * 开发人员能集中精力在核心逻辑上。比如每个业务流程都有日志和权限验证的功能，还可能
 * 增加新的功能，实际上我们只关心核心逻辑，其他的一些附加逻辑，如日志和权限不需要关注，
 * 这时，就可以将日志和权限等非核心逻辑“横切”出来，使核心逻辑尽可能保持简洁和清晰，方便维护。
 * 这样“横切”的另一个好处是，这些公共的非核心逻辑被提取到多个切面中了，使他们可以被其他组件或
 * 对象复用，消除了重复代码。
 *
 * AOP把软件系统分为两个部分：核心关注点和横向关注点。业务处理的主要流程是核心关注点，与之
 * 关系不大的部分是横切关注点。横切关注点的一个特点是，他们经常发生在核心关注点的多处，而各处
 * 都基本相似，比如权限认证，日志，事务处理。AOP的作用在于分离系统中的各种关注点，将核心关注点和横切关注点分离开。
 */
#ifndef IPERFOUTPUTDATAANALYSIS_ASPECT_H
#define IPERFOUTPUTDATAANALYSIS_ASPECT_H
/**
 * 这里使用模板来做约束，即每个切面对象必须有Before(Args...)或After(Args...)方法，
 * 用来处理核心逻辑执行前后的非核心逻辑。
 *
 * 参数是可变的，支持1到N（N>0)切面
 */
#include <type_traits>

#define HAS_MEMBER(member)\
template<typename T, typename... Args>struct has_member_##member\
{\
private:\
		template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	template<typename U> static std::false_type Check(...);\
public:\
	enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(Foo)
HAS_MEMBER(Before)
HAS_MEMBER(After)

#include <NonCopyable.hpp>
#include <utility>
#include "NonCopyable.hpp"

template<typename Func, typename... Args>
struct Aspect : NonCopyable{
    Aspect(Func&& f) : m_func(std::forward<Func>(f)){

    }

    template<typename T>
    typename std::enable_if<has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect){
        aspect.Before(std::forward<Args>(args)...);//核心逻辑之前的切面逻辑
        m_func(std::forward<Args>(args)...);//核心逻辑
        aspect.After(std::forward<Args>(args)...);//核心逻辑之后的切面逻辑
    };

    template<typename T>
    typename std::enable_if<has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect){
        aspect.Before(std::forward<Args>(args)...);//核心逻辑之前的切面逻辑
        m_func(std::forward<Args>(args)...);
    };

    template<typename T>
    typename std::enable_if<!has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
    {
        m_func(std::forward<Args>(args)...);//核心逻辑
        aspect.After(std::forward<Args>(args)...);//核心逻辑之后的切面逻辑
    }




private:
    Func m_func;
};
#endif //IPERFOUTPUTDATAANALYSIS_ASPECT_H
