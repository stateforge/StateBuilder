#ifndef __WASHING_MACHINE__
#define __WASHING_MACHINE__

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class WashingMachineContext;


class WashingMachine
{

public:
	WashingMachine();

	~WashingMachine();
    
    void Process();
    
    int GetRtxCounter() const {return iRtxCount;}
    
    // Events
	void Start();
	void WashingDone();
    void RinsingDone();
    void SpinningDone();
    void Fault();
	void DiagnoseSuccess();

    // Actions
	void Print(const string& message);
    
  private:

    WashingMachineContext& GetContext();
    WashingMachineContext *m_pContext;
    int iRtxCount;
	int iCounter;
};

#endif
