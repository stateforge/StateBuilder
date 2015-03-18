#ifndef __DOORTESTER_H__
#define __DOORTESTER_H__

#include <cstdlib>
#include <iostream>

#include <string>
#include "Door.h"

using namespace std;

class DoorTesterContext;

class DoorTester 
{

public:
    DoorTester(Door &door, boost::asio::io_service &io_service);

    ~DoorTester();

    void EvStartTest();

    Door& GetDoor(){return m_door;}

    unsigned int GetOpenDuration(){return 1000;};

private:
    //FRIEND_CLASS_FSM

    DoorTester& operator= (const DoorTester &other);

    DoorTesterContext& GetContext();
    DoorTesterContext *m_pContext;
    Door &m_door;
};

#endif
