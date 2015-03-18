#ifndef HELLOWORLD_H_
#define HELLOWORLD_H_

#include <string>
#include <fsm/IManagement.h>

#define HELLO "Ciao"
#define BYE "Bye"

class HelloWorldContext; //Forward declaration

class HelloWorld : public fsm::IManagement
{
public:
    HelloWorld();
    virtual ~HelloWorld();
    
    void start();
    void stop();
    
    void print(const std::string &message);

  private:
    void OnEnd(); // Override fsm::IManagement::OnEnd()
    HelloWorldContext *m_pContext;// Generated class
};

#endif
