#ifndef Robot_H
#define Robot_H

#include <QtCore/QObject>

class QTimer;
class RobotContext; // Generated class
class Arm;
class Camera;

class Robot : public QObject
{
    Q_OBJECT

public:
    Robot();
    virtual ~Robot();

public slots:
    void start();
    void stop();

private slots:
    void contextEnd();

private:
    RobotContext *m_pRobotContext; // Generated class
    Arm *m_pArm;
    Camera *m_pCamera;
    void logInit();
};

#endif // Robot_H
