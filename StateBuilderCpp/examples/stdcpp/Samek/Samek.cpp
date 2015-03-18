
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <ObserverStd.h>

#include "Samek.h"

using namespace std;

const int RTX_COUNT = 5;

Samek::Samek()
: foo(0)
{
    m_pContext = new SamekContext(*this);
    m_pContext->SetObserver(fsm::ObserverStd::GetInstance());
    m_pContext->EnterInitialState();
}

Samek::~Samek()
{
    delete m_pContext;

}
SamekContext& Samek::GetContext()
{
    if(m_pContext != NULL){
        return *m_pContext;
    } else {
        throw runtime_error("context is not set");
    }
}

void Samek::a()
{
    GetContext().a();
}

void Samek::b()
{
    GetContext().b();
}

void Samek::c()
{
    GetContext().c();
}

void Samek::d()
{
    GetContext().d();
}
void Samek::e()
{
    GetContext().e();
}
void Samek::f()
{
    GetContext().f();
}
void Samek::g()
{
    GetContext().g();
}

void Samek::h()
{
    GetContext().h();
}
void Samek::Print(const std::string &message)
{
    cout << message;
}
