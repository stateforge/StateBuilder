#ifndef LED_H_
#define LED_H_

class LedContext;
class LedPrivate;

class Led
{
public:
    Led();

    virtual ~Led();
    
    void On();
    void Off();
    
  private:

    LedPrivate *m_pPrivate;// Actions implementation class
    LedContext *m_pContext;// Generated class
};

#endif
