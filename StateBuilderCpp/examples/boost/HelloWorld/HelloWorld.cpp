#include <iostream>
#include <logging.hpp>
#include <SmBoostObserver.h>

#include "HelloWorld.h"
#include "HelloWorldFsm.h"

using namespace std;

const int RTX_COUNT = 2;

HelloWorld::HelloWorld(boost::asio::io_service &io_service)
: iRtxCount(RTX_COUNT), 
  iCounter(0)
{
    m_pContext = new HelloWorldContext(*this, io_service);
    m_pContext->SetObserver(SmBoostObserver::GetInstance());
    m_pContext->EnterInitialState();
    m_pContext->EvStart();
}

HelloWorld::~HelloWorld()
{
    delete m_pContext;
}

void HelloWorld::Print(const std::string& message)
{
    BOOST_LOG_L2(message);
}

bool HelloWorld::HasReachedLimit()
{
    if(iCounter == iRtxCount){
        return true;
    }else {
        return false;
    }
}
