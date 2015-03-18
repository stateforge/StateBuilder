#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include "TrafficLight.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    TrafficLight trafficLight;
    QTimer::singleShot(0,&trafficLight, SLOT(start()));
    return app.exec();
}
