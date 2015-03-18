#include <stdexcept>
#include <logging.hpp>
#include <SmBoostObserver.h>
#include "Microwave.h"
#include "Microwave02Fsm.h"
#include <iostream>

using namespace std;

Microwave::Microwave() :
m_bRun(true),
m_eventsCountDoor(0),
m_eventsCountUser(0)
{
    m_pContext = new MicrowaveContext(*this);
    m_pContext->SetObserver(SmBoostObserver::GetInstance());
    m_pContext->EnterInitialState();
}

Microwave::~Microwave()
{
    delete m_pContext;
}

void Microwave::doorWorker()
{
    while(m_bRun == true){
        m_pContext->evDoorOpened();
        m_pContext->evDoorClosed();
        m_eventsCountDoor += 2;
    }
}

void Microwave::userWorker()
{
    while(m_bRun == true){
        m_pContext->evTurnOn();
        m_pContext->evTurnOff();
        m_pContext->evCookingDone();
        m_eventsCountUser += 3;
    }
}

void Microwave::workersStart()
{
    m_threadDoor = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Microwave::doorWorker, this)));
    m_threadUser = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Microwave::userWorker, this)));
}

void Microwave::workersStop()
{
    m_bRun = false;
    m_threadUser->join();
    m_threadDoor->join();
    BOOST_LOG_L3("Door " << m_eventsCountDoor);
    BOOST_LOG_L3("User " << m_eventsCountUser);
    BOOST_LOG_L3("Total " <<  m_eventsCountDoor + m_eventsCountUser );
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
    cout << message << endl;
}
