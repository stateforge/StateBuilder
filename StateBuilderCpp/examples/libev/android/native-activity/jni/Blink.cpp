/*
 *  Blink.cpp
 */

#include "Blink.h"
#include "BlinkFsm.h"
#include "SmAndroidObserver.h"

Blink::Blink(ev::default_loop &loop)
{
     m_pContext = new BlinkContext(*this, loop);
     m_pContext->SetObserver(SmAndroidObserver::GetInstance());
     m_pContext->EnterInitialState();
}

Blink::~Blink()
{
    delete m_pContext;
}

void Blink::start()
{
     m_pContext->EvStart();
}
