#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QtCore/QObject>

class TrafficLightContext; // Generated class
class Light;

class TrafficLight : public QObject
{
    Q_OBJECT
    
public:
    TrafficLight();
    virtual ~TrafficLight();

public slots:
    void start();
  
private slots:
    void contextEnd();

private:
    TrafficLightContext *m_pTrafficLightContext; // Generated class
    Light *m_pLight;
    void logInit();
};

#endif // TRAFFICLIGHT_H
