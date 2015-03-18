#include <cstring>
#include <fsm/IObserver.h>
#include "IdObserver.h"


IdObserver::IdObserver(const fsm::StateNameToId *pStateNameToId)
    : _pStateNameToId(pStateNameToId)
{
}

void IdObserver::OnEntry(const char */*pContextName*/, const char *pStateName)
{
    int iStateId = getIdFromName(pStateName);
};

void IdObserver::OnExit(const char */*pContextName*/, const char *pStateName)
{
    int iStateId = getIdFromName(pStateName);
};

void IdObserver::OnTransitionBegin(const char */*pContextName*/,
    const char *pStatePrevious,
    const char *pStateNext,
    const char */*pTransitionName*/) 
{
    int iStateIdPrevious = getIdFromName(pStatePrevious);
    int iStateIdNext = getIdFromName(pStateNext);
}

void IdObserver::OnTransitionEnd(const char */*pContextName*/,
    const char *pStatePrevious,
    const char *pStateNext,
    const char *pTransitionName) 
{
    int iStateIdPrevious = getIdFromName(pStatePrevious);
    int iStateIdNext = getIdFromName(pStateNext);

}


int IdObserver::getIdFromName(const char* pStateName)
{
    const fsm::StateNameToId *pStateNameToId = _pStateNameToId;
    while(pStateNameToId->pcName){
        if(std::strcmp(pStateName, pStateNameToId->pcName) == 0){
            return _pStateNameToId->iId;
        }
        pStateNameToId++;
    }

    return 0;
}
