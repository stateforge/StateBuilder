#ifndef LIGHT_H
#define LIGHT_H

#include <QtCore/QObject>

class Light : public QObject
{
    Q_OBJECT
public:
    Light(); 
    virtual ~Light();

    long getRedDuration() const {return m_lRedDuration;}
    long getYellowDuration() const {return m_lYellowDuration;}
    long getGreenDuration() const {return m_lGreenDuration;}
    long getMaxDuration() const {return m_lMaxDuration;}

    void TurnOnRed();
    void TurnOffRed();

    void TurnOnYellow();
    void TurnOffYellow();
    
    void TurnOnGreen();
    void TurnOffGreen();

private:
    long m_lRedDuration;
    long m_lYellowDuration;
    long m_lGreenDuration;
    long m_lMaxDuration;
};

#endif // TRAFFICLIGHT_H
