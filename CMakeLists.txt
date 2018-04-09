cmake_minimum_required(VERSION 3.7)
project(IperfOutputDataAnalysis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(SOURCE_FILES main.cpp Tools.cpp Tools.h zbInfoNode.cpp zbInfoNode.h SyncQueue.hpp ThreadPool.hpp TaskFunctionCollections.hpp PreIO.hpp Aspect.hpp NonCopyable.hpp TimeElapsedAspect.hpp Timer.hpp LoggingAspect.hpp MainWork.hpp Ioc.hpp)
add_executable(IperfOutputDataAnalysis ${SOURCE_FILES})