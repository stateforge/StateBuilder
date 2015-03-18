/*
 *  Blink.h
 *
 */
#include <ev++.h>

class BlinkContext;

class Blink 
{
public:
    Blink(ev::default_loop &loop);
    virtual ~Blink();
    void start();
private:
    BlinkContext *m_pContext;
};
