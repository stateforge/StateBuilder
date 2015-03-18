#include <stdexcept>
#include <ObserverStd.h>
#include "WashingMachine.h"
#include "WashingMachineFsm.h"
#include <iostream>

using namespace std;

WashingMachine::WashingMachine()
{
    m_pContext = new WashingMachineContext(*this);
    m_pContext->SetObserver(fsm::ObserverStd::GetInstance());
    m_pContext->EnterInitialState();
}

WashingMachine::~WashingMachine()
{
    delete m_pContext;
}

WashingMachineContext& WashingMachine::GetContext()
{
    if(m_pContext != NULL){
        return *m_pContext;
    } else {
        throw std::runtime_error("context is not set");
    }
}

void WashingMachine::Start()
{
    GetContext().Start();
}

void WashingMachine::WashingDone()
{
    GetContext().WashingDone();
}

void WashingMachine::RinsingDone()
{
    GetContext().RinsingDone();
}

void WashingMachine::SpinningDone()
{
    GetContext().SpinningDone();
}

void WashingMachine::Fault()
{
    GetContext().Fault();
}

void WashingMachine::DiagnoseSuccess()
{
    GetContext().DiagnoseSuccess();
}

void WashingMachine::Print(const std::string& message)
{
    cout << message << endl;
}
