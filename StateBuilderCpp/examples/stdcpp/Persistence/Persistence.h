#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include <string>
#include <fsm/IManagement.h>

#define HELLO "Ciao"
#define BYE "Bye"

class Persistence01Context; //Forward declaration


class Action {
public:
    Action();
    virtual ~Action();

    void print(const std::string &message);

  private:
};

class Persistence : public fsm::IManagement
{
public:
    Persistence();
    virtual ~Persistence();
    
    void start();
    void stop();
    
    void print(const std::string &message);

  private:
    void OnEnd(); // Override fsm::IManagement::OnEnd()
    Persistence01Context *m_pContext;// Generated class
};

#endif
