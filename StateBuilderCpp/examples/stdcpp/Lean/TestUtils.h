#ifndef TESTUTILS_H
#define TESTUTILS_H
#include <fsm/IManagement.h>

// The purpose of Management is to know when state machine enters the final state
// Management is OPTIONAL
class Management : public fsm::IManagement 
{
public:
    Management(): bEnd(false) {}
    //Implements fsm::IManagement::OnEnd()
    void OnEnd(){bEnd = true;}
    bool IsEnd() {return bEnd;}

private:
    bool bEnd;
};

template<class TContext>
void processMachine(TContext &context)
{
    //Find out when the state machine ends. Optional
    Management management;
    context.SetManagement(&management);

    // Feed the state machine with events until it reaches the final state
    while(!management.IsEnd()){
        context.eventA();
        context.eventB();
        context.eventC();
    }
}

#endif