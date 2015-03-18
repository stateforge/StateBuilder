#include <QtCore/QTimer>
#include "Arm.h"

Arm::Arm(void)
{
}

Arm::~Arm(void)
{
} 

void Arm::calibrateRequest(void)
{
    QTimer::singleShot(200, this, SIGNAL(calibrationDone()));
} 

void Arm::onCalibrationDone()
{
    emit calibrationDone();
}

#include "Arm.moc"