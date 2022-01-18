#include "Schedule.h"

using namespace tinyco;

Schedule *Schedule::instance_ = nullptr;
std::mutex Schedule::m_mutex;


Schedule::Schedule(): isFirstRun(true), lastCoroutine_(nullptr), curCoroutine_(nullptr)
{
    context_.makeCurContext();
}


Schedule::~Schedule()
{

}

Schedule *Schedule::getInstance()
{
    if (instance_ == nullptr) {
        std::unique_lock<std::mutex>m_lock(m_mutex);
        if (instance_ == nullptr) {
            instance_ = new Schedule;
        }
    }
    return instance_;
}

//恢复指定的协程
void Schedule::resume(Coroutine *pCo) 
{ 
    if (nullptr == pCo) {
        return;
    }
    lastCoroutine_ = curCoroutine_;
    if (lastCoroutine_) { //判断以前的协程是否存在
        lastCoroutine_->yield();    //如果存在则等待
        coroutines_.push(lastCoroutine_);//将以前的协程加入队列中以后执行
    }
    curCoroutine_ = pCo; //当前协程等于传递进来的协程
    if (isFirstRun) {
        isFirstRun = false;
        pCo->resume(true);
    } else {
        pCo->resume();
    }
}

void Schedule::runNewCo(std::function<void()>&func) 
{
    Coroutine * pCo = new Coroutine(this, func);
    resume(pCo);
}

void Schedule::runNewCo(std::function<void()>&&func) 
{
    Coroutine * pCo = new Coroutine(this, std::move(func));
    resume(pCo);
}

void Schedule::killCurCo() {
    delete curCoroutine_;
    curCoroutine_ = nullptr;
}

void Schedule::resumeAnotherCoroutine() 
{
    if (coroutines_.empty()) {
        isFirstRun = true;
        context_.swapContext(nullptr);
    } 
    auto & co = coroutines_.front();
    coroutines_.pop();
    resume(co);
}