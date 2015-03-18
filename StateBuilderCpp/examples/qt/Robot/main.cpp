#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include "Robot.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    Robot robot;
    QTimer::singleShot(0,&robot, SLOT(start()));
    QTimer::singleShot(1000,&robot, SLOT(stop()));
    return app.exec();
}
