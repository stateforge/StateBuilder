#ifndef DOOR_H
#define DOOR_H

#include <cstdlib>
#include <iostream>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include <string>

using namespace std;

class DoorContext;
class DoorPrivate;

class Door
{
    friend class DoorPrivate;
public:
	Door(boost::asio::io_service &io_service);

	~Door();

	void EvOpen();
	void EvClose();

	void EvSensorOpened();
	void EvSensorClosed();

    void EvQuit();

private:

    DoorContext *m_pContext;
    DoorPrivate *m_pPrivate;
    
};

#endif
