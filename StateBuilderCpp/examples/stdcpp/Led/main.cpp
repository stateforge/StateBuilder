#include <iostream>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include "Led.h"
#include "LedFsm.h"

using namespace std;

int getIdFromName(const char* pStateName)
{
    const fsm::StateNameToId *pStateNameToId = LedContext::GetStateNameToId();
    while(pStateNameToId->pcName){
        if(std::strcmp(pStateName, pStateNameToId->pcName) == 0){
            return pStateNameToId->iId;
        }
        pStateNameToId++;
    }

    return 0;
}

void testStateNameToId()
{
    assert(0 == getIdFromName("Led"));
    assert(1 == getIdFromName("SwitchedOff"));
    assert(2 == getIdFromName("SwitchedOn"));
    assert(0 == getIdFromName("IDoNotExist"));
}

int main(int /*argc*/, char ** /*argv[]*/)
{
  int iReturnValue = 0;

  testStateNameToId();

  cout << "Led" << endl; 
  try {
    Led led;
    led.On();
    led.Off();
  }
  catch (std::exception& e){
    cerr << "Exception: " << e.what() << endl;
    iReturnValue = -1;
  }
  return iReturnValue;
}
