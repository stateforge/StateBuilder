#ifndef __WASHING_MACHINE__
#define __WASHING_MACHINE__

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

using namespace std;

class MicrowaveContext;

class Microwave
{

public:
    Microwave();

    ~Microwave();

    //Start the worker threads
    void workersStart();

    //Stop the worker threads
    void workersStop();

    int GetRtxCounter() const {return iRtxCount;}

    //Events
    void evStart();
    void evStop();
    void evTurnOn();
    void evTurnOff();
    void evCookingDone();
    void evDoorOpened();
    void evDoorClosed();

    //Actions
    void Print(const string& message);

private:

    MicrowaveContext& GetContext();
    MicrowaveContext *m_pContext;
    bool m_bRun;
    int iRtxCount;
    int iCounter;
    void doorWorker();
    void userWorker();

    unsigned int m_eventsCountDoor;
    unsigned int m_eventsCountUser;
    boost::shared_ptr<boost::thread> m_threadDoor;
    boost::shared_ptr<boost::thread> m_threadUser;

};

#endif
