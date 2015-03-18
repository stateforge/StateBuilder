#ifndef FSM_TEST_H
#define FSM_TEST_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <string>


class FsmTestContext;

using namespace std;

class FsmTest
{

public:
    FsmTest(boost::asio::io_service &io_service);
    // Default constructor.

    virtual ~FsmTest();

    void Print(const string &name);

    int GetRtxCounter() const {return iRtxCount;}

    //bool EventProcessed();

private:


    unsigned int m_uiTick;
    int iRtxCount;
    FsmTestContext  *m_pContext;
};

#endif
