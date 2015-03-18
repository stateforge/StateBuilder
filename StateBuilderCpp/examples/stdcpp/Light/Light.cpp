#include <ObserverStd.h>
#include "Light.h"

#include "LightFsm.h" //Generated header
#include "LightPrivate.h"

using namespace std;

Light::Light()
{
    m_pPrivate = new LightPrivate();
    m_pContext = new LightContext(*m_pPrivate);
    m_pContext->SetObserver(fsm::ObserverStd::GetInstance());
    m_pContext->EnterInitialState();
}

Light::~Light()
{
    delete m_pContext;
    delete m_pPrivate;
}

void Light::on()
{
    m_pContext->evOn();
}

void Light::off()
{
    m_pContext->evOff();
}
