
#include <SmBoostObserver.h>

#include "Door.h"
#include "DoorPrivate.h"
#include "DoorFsm.h"

using namespace std;

Door::Door(boost::asio::io_service &io_service)
{
    m_pPrivate = new DoorPrivate(); 
    m_pContext = new DoorContext(*m_pPrivate, io_service);
    m_pContext->SetObserver(SmBoostObserver::GetInstance());
    m_pContext->EnterInitialState();
}

Door::~Door() 
{
    delete m_pContext;
    delete m_pPrivate;
}

void Door::EvOpen()
{
    m_pContext->EvOpen();
}

void Door::EvClose()
{
    m_pContext->EvClose();
}

void Door::EvSensorOpened()
{
    m_pContext->EvSensorOpened();
}

void Door::EvSensorClosed()
{
    m_pContext->EvSensorClosed();
}

void Door::EvQuit()
{
    m_pContext->EvQuit();
}
