#ifndef LIGHT_H_
#define LIGHT_H_

#include <cstdlib>

class LightContext;
class LightPrivate;

class Light
{
public:
    Light();

    virtual ~Light();
    
    void on();
    void off();

  private:

    LightContext *m_pContext;// Generated class
    LightPrivate *m_pPrivate;
};

#endif
