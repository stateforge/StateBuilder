#ifndef CAMERA_H
#define CAMERA_H

#include <QtCore/QObject>

class Camera : public QObject
{
     Q_OBJECT
public:

    Camera(void);
    ~Camera(void);

    void calibrateRequest();

signals:
    void calibrationDone();
};

#endif