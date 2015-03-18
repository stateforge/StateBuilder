#include <cstdlib>
#include <iostream>
#include <ObserverStd.h>
#include "HelloWorld.h"
#include "HelloWorldFsm.h" // Generated file

using namespace std;

HelloWorld::HelloWorld()
{
    cout << "HelloWorld::ctor()" << endl;
    m_pContext = new HelloWorldContext(*this);
    m_pContext->SetObserver(fsm::ObserverStd::GetInstance());
    m_pContext->EnterInitialState();
    m_pContext->SetManagement(this);
}

HelloWorld::~HelloWorld()
{
    cout << "HelloWorld::dtor()" << endl;
    delete m_pContext;
}

void HelloWorld::start()
{
    cout << "HelloWorld::start()" << endl;
    m_pContext->evStart();
}

void HelloWorld::stop()
{
    cout << "HelloWorld::stop()" << endl;
    m_pContext->evStop();
}

void HelloWorld::print(const std::string &message)
{
    cout << message << endl;
}

void HelloWorld::OnEnd()
{
    cout << "HelloWorld::OnEnd() context has ended" << endl;
}
