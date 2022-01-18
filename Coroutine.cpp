#include "Coroutine.h"
#include "Schedule.h"
using namespace tinyco;

static void coWrapFunc(Schedule *schedule) 
{
    schedule->getCurCoroutine()->run(); //先执行函数
    schedule->killCurCo();//killself
    schedule->resumeAnotherCoroutine();//唤醒别的协程
}

Coroutine::Coroutine(Schedule *schedule, std::function<void()>&func) : status_(COROUTINE_READY), ctx_(), schedule_(schedule),func_(func)
{

}

Coroutine::Coroutine(Schedule *schedule, std::function<void()>&&func) : status_(COROUTINE_READY), ctx_(), schedule_(schedule), func_(std::move(func))
{

}

void Coroutine::yield()
{
    status_ = COROUTINE_WATING;
}


void Coroutine::resume(bool flag)
{
    Coroutine *lastCo = schedule_->getLastCoroutine();
    switch (status_)
    {
    case COROUTINE_READY:
        status_ = COROUTINE_RUNNING;
        ctx_.makeContext((void(*)(void))coWrapFunc, schedule_);
        if (flag) {
            ctx_.swapContext(schedule_->getContext());
        } else {
            ctx_.swapContext(lastCo ? lastCo->getCtx() : nullptr);
        }
        break;
    case COROUTINE_WATING:
        status_ = COROUTINE_RUNNING;
        ctx_.swapContext(lastCo ? lastCo->getCtx() : nullptr);
        break;
    default:
        break;
    }
}