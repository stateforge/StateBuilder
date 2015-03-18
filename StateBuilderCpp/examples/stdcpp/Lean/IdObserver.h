#ifndef IDOBSERVER_H
#define IDOBSERVER_H

#include <fsm/State.h>
#include <fsm/IObserver.h>

class IdObserver : public fsm::IObserver 
{
public:
    IdObserver(const fsm::StateNameToId *pStateNameToId);

    void OnEntry(const char *pContextName, const char *pStateName);

    void OnExit(const char *pContextName, const char *pStateName);

    void OnTransitionBegin(const char *pContextName,
        const char *pStatePrevious,
        const char *pStateNext,
        const char *pTransitionName);

    void OnTransitionEnd(const char *pContextName,
        const char *pStatePrevious,
        const char *pStateNext,
        const char *pTransitionName);

private:
    int getIdFromName(const char* pStateName);
    const fsm::StateNameToId * const _pStateNameToId;
};


#endif