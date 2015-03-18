#include <string>
#include <SmBoostObserver.h>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <logging.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include "FsmTest.h"
#include "FsmTestM1S3Fsm.h"

using namespace std;
namespace bl = boost::logging;

FsmTest::FsmTest(boost::asio::io_service &io_service)
:
m_uiTick(0), 
m_pContext(NULL)
{
    m_pContext = new FsmTestContext(*this, io_service);
    m_pContext->SetObserver(SmBoostObserver::GetInstance());
    m_pContext->EvStart();
}

FsmTest::~FsmTest()
{
    delete m_pContext;
}


void FsmTest::Print(const std::string &message)
{
    BOOST_LOG_L3(message);
}
