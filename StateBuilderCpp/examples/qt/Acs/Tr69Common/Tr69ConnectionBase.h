#ifndef TR69CONNECTIONBASE_H
#define TR69CONNECTIONBASE_H

#include <QtCore/QObject>
#include <QtCore/QString>

/**
  @class AcsConnection
*/
class Tr69ConnectionBase : public QObject
{
  Q_OBJECT
    
  public:
    Tr69ConnectionBase(QObject *pParent);
    virtual ~Tr69ConnectionBase();
 
    const QString getErrorMessage() {return m_errorMessage;};

protected:
    QString m_errorMessage;
};

#endif
