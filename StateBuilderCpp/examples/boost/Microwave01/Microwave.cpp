#include <stdexcept>
#include <logging.hpp>
#include <SmBoostObserver.h>
#include "Microwave.h"
#include "MicrowaveFsm.h"
#include <iostream>

using namespace std;

Microwave::Microwave()
{
    m_pContext = new MicrowaveContext(*this);
    m_pContext->SetObserver(SmBoostObserver::GetInstance());
    m_pContext->EnterInitialState();
}

Microwave::~Microwave()
{
    delete m_pContext;
}

MicrowaveContext& Microwave::GetContext()
{
    if(m_pContext != NULL){
        return *m_pContext;
    } else {
        throw std::runtime_error("context is not set");
    }
}

void Microwave::evStart()
{
    GetContext().evStart();
}

void Microwave::evStop()
{
    GetContext().evStop();
}

void Microwave::evTurnOn()
{
    GetContext().evTurnOn();
}

void Microwave::evTurnOff()
{
    GetContext().evTurnOff();
}

void Microwave::evCookingDone()
{
    GetContext().evCookingDone();
}

void Microwave::evDoorOpened()
{
    GetContext().evDoorOpened();
}

void Microwave::evDoorClosed()
{
    GetContext().evDoorClosed();
}

void Microwave::Print(const std::string& message)
{
    BOOST_LOG_L3(message);
}
