#include <iostream>
#include <ObserverStd.h>
#include "Led.h"
#include "LedPrivate.h"
#include "LedFsm.h" // Generated file

using namespace std;

// Led implementation
Led::Led()
{
    m_pPrivate = new LedPrivate();
    m_pContext = new LedContext(*m_pPrivate);
    m_pContext->SetObserver(fsm::ObserverStd::GetInstance());
    m_pContext->EnterInitialState();
}

Led::~Led()
{
    delete m_pContext;
    delete m_pPrivate;
}

void Led::On()
{
    m_pContext->On();
}

void Led::Off()
{
    m_pContext->Off();
}

// LedPrivate implementation
void LedPrivate::DoOn()
{
    cout << "DoOn" << endl;
}

void LedPrivate::DoOff()
{
    cout << "DoOff" << endl;
}