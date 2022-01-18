#pragma once
#include <mutex>
#include <queue>
#include <functional>
#include "Utils.h"
#include "Coroutine.h"
#include "Context.h"


#define co_yield() tinyco::Schedule::getInstance()->resumeAnotherCoroutine()
#define co_go(func) tinyco::Schedule::getInstance()->runNewCo(func)
namespace tinyco
{
    class Schedule 
    {
        DELETE_ALL_COPY_CONSTRUTION(Schedule);//单例模式
        public:
            static Schedule * getInstance();

            //创建新协程
            void runNewCo(std::function<void()>&func);
            void runNewCo(std::function<void()>&&func);

            //恢复其它协程
            void resumeAnotherCoroutine();
            
            /*清除当前正在运行的协程*/
            void killCurCo();

            //获取最后运行的协程
            Coroutine * getLastCoroutine() {return lastCoroutine_;}

            //获取当前运行的协程
            Coroutine * getCurCoroutine() {return curCoroutine_;}

            //获取上下文
            Context* getContext() {return &context_;}
            
        private:
            Schedule();
            ~Schedule();
            static Schedule *instance_;
            std::queue<Coroutine*> coroutines_;
            bool isFirstRun;
            Coroutine * lastCoroutine_;
            Coroutine * curCoroutine_;
            Context context_;

            static std::mutex m_mutex;

            /*恢复指定协程*/
            void resume(Coroutine *);
    };
}