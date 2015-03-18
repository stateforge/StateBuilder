#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include "UrlGet.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    UrlGet urlGet;
    QTimer::singleShot(0,&urlGet, SLOT(start()));
    return app.exec();
}
