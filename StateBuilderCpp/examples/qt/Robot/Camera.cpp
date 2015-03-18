#include <QtCore/QTimer>
#include "Camera.h"


Camera::Camera(void)
{
}

Camera::~Camera(void)
{
} 

void Camera::calibrateRequest(void)
{
    QTimer::singleShot(400, this, SIGNAL(calibrationDone()));
} 

#include "Camera.moc"