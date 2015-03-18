
#include <SmBoostObserver.h>
#include "DoorTester.h"
#include "DoorTesterFsm.h"

using namespace std;

DoorTester::DoorTester(Door &door, boost::asio::io_service &io_service) : m_door(door)
{
    m_pContext = new DoorTesterContext(*this, io_service);
    m_pContext->SetObserver(SmBoostObserver::GetInstance());
    m_pContext->EnterInitialState();
}

DoorTester::~DoorTester() 
{
    delete m_pContext;
}

DoorTesterContext& DoorTester::GetContext()
{
    if(m_pContext != NULL){
        return *m_pContext;
    } else {
        throw std::runtime_error("context is not set");
    }
}

void DoorTester::EvStartTest()
{
    GetContext().EvStartTest();
}
