#ifndef __WASHING_MACHINE__
#define __WASHING_MACHINE__

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class MicrowaveContext;


class Microwave
{

public:
	Microwave();

	~Microwave();

	void Process();

	int GetRtxCounter() const {return iRtxCount;}

	void evStart();
	void evStop();
	void evTurnOn();
	void evTurnOff();
	void evCookingDone();
	void evDoorOpened();
	void evDoorClosed();

	void Print(const string& message);

private:
	//FRIEND_CLASS_FSM

	MicrowaveContext& GetContext();
	MicrowaveContext *m_pContext;
	int iRtxCount;
	int iCounter;
};

#endif
