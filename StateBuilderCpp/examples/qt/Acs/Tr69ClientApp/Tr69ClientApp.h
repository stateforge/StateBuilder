#include <QtCore/QObject>

class Tr69Client;

class Tr69ClientApp : public QObject {
  
  Q_OBJECT 
  
  public:
    
    Tr69ClientApp();
    
    void start();

private:
    Tr69Client *m_pTr69Client;
};

