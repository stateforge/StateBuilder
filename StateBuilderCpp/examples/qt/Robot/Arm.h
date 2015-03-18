#ifndef ARM_H
#define ARM_H

#include <QtCore/QObject>

class Arm : public QObject
{
     Q_OBJECT
public:

    Arm(void);
    ~Arm(void);

    void calibrateRequest();

signals:
    void calibrationDone();

private slots:
    void onCalibrationDone();
};

#endif