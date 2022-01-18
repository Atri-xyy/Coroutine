#pragma once
#include <functional>
#include <stdlib.h>
#include "Utils.h"
#include "Context.h"
#include "Common.h"

namespace tinyco 
{
    enum coStatus {
        COROUTINE_READY,
        COROUTINE_RUNNING,
        COROUTINE_WATING,
        COROUTINE_DEAD
    };
    class Schedule;
    class Coroutine 
    {
        public:
            Coroutine(){}
            ~Coroutine(){}

            Coroutine(Schedule*, std::function<void()>&);
            Coroutine(Schedule*, std::function<void()>&&);

            DELETE_ALL_COPY_CONSTRUTION(Coroutine);
            //恢复当前的协程
            void resume(bool flag = false);

            //暂停当前的协程
            void yield();

            //执行协程函数
            void run() {func_();}

            //获取协程上下文
            Context *getCtx(){return &ctx_;}
        private:
            int status_;
            Context ctx_;
            Schedule *schedule_;
            std::function<void()>func_;
    };
}