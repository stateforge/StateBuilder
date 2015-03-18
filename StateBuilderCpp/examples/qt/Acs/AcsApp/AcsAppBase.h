#ifndef ACSAPPBASE_H
#define ACSAPPBASE_H

#include <QtCore/QObject>
#include <Acs.h>

class AcsAppBase : public QObject {
  
  Q_OBJECT 
  
  public:
    
    AcsAppBase(const QString &host);
    virtual ~AcsAppBase();
    
    void start();
    
  public slots:
    
    void onClientEnd(AcsClientToken &clientToken, bool bError);
    
  protected:
    
    Acs m_acs;
    AcsClientToken m_clientToken;
};

#endif // ACSAPPBASE_H
