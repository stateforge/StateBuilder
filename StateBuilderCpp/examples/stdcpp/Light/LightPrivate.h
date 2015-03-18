#ifndef LIGHTPRIVATE_H
#define LIGHTPRIVATE_H

#include <cstdlib>


using namespace std;

class LightPrivate
{
public:
    LightPrivate();

    virtual ~LightPrivate();

    void doOn();
    void doOff();
};

#endif
