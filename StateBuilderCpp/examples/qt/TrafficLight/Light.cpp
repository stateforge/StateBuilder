

#include <QsLog.h>
#include <QsLogDest.h>

#include <Light.h>

Light::Light() :
  m_lRedDuration(1000),
  m_lYellowDuration(1000),
  m_lGreenDuration(1000),
  m_lMaxDuration(3500)
{
}

Light::~Light()
{
}

void Light::TurnOnRed()
{
  QLOG_INFO();
}

void Light::TurnOffRed()
{
  QLOG_INFO();
}

void Light::TurnOnYellow()
{
  QLOG_INFO();
}

void Light::TurnOffYellow()
{
  QLOG_INFO();
}

void Light::TurnOnGreen()
{
  QLOG_INFO();
}

void Light::TurnOffGreen()
{
  QLOG_INFO();
}

#include "Light.moc"
