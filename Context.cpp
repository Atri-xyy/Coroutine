#include "Context.h"


using namespace tinyco;

Context::Context() : pCtx_(nullptr), pStack_(nullptr)
{

}

Context::~Context() 
{
    if (pCtx_) {
        delete pCtx_;
    }

    if (pStack_) {
        free(pStack_);
    }
}

void Context::makeCurContext() 
{
    if (pCtx_ == nullptr) {
        pCtx_ = new struct ucontext_t;
    }
    ::getcontext(pCtx_);
}

void Context::swapContext(Context * oldCtx) 
{
    if (oldCtx == nullptr) {
        setcontext(pCtx_);
    } else {
        swapcontext(oldCtx->getContext(), pCtx_);
    }
}

void Context::makeContext(void(*func)(), Schedule* pSche) 
{
    if (pCtx_ == nullptr) {
        pCtx_ = new struct ucontext_t;
    }
    if (pStack_ == nullptr) {
        pStack_ = malloc(parameter::coroutineStackSize);
    }
    ::getcontext(pCtx_);
    pCtx_->uc_stack.ss_sp = pStack_;
    pCtx_->uc_stack.ss_size = parameter::coroutineStackSize;
    pCtx_->uc_link = nullptr;
    makecontext(pCtx_, func, 1, pSche);
}