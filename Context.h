#pragma once
#include <ucontext.h>
#include <stdlib.h>
#include "Utils.h"
#include "Common.h"
#include <iostream>
#include <string.h>
namespace tinyco 
{
    class Schedule;
    class Context 
    {
        public:
            Context();
            ~Context();

            Context(const Context & otherCtx) : pCtx_(otherCtx.pCtx_), pStack_(otherCtx.pStack_){}

            Context(const Context && otherCtx) : pCtx_(otherCtx.pCtx_), pStack_(otherCtx.pStack_){}

            Context & operator = (const Context &) = delete;

            //用函数指针设置当前上下文的入口    
            void makeContext(void(*func)(), Schedule*);

            //用当前程序状态设置context上下文
            void makeCurContext();

            //将当前上下文和oldCtx切换
            void swapContext(Context * oldCtx);

            //获取当前上下文
            struct ucontext_t* getContext() {return pCtx_;}

        private:
            struct ucontext_t* pCtx_;
            void* pStack_;
    };
}

