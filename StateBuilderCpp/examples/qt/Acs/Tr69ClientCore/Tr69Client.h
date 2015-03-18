#ifndef TR69CLIENT_H
#define TR69CLIENT_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>
#include <QtSoapMessage>

class Tr69ClientPrivate;

/**
  * @class Tr69ClientConf
  */
class Tr69ClientConf
{
public:
    Tr69ClientConf()
       {}
};

/**
  * @class Tr69Client
  */
class Tr69Client : public QObject
{
  Q_OBJECT

  public:
    Tr69Client(QObject *pParent = NULL);
    ~Tr69Client();
    
    Tr69ClientConf& conf();
    const QUrl& acsUrl();

  public slots:
    void start();
    void stop();

  Q_SIGNALS:

  private:
    friend class Tr69ClientPrivate;
    Tr69ClientPrivate *m_pPrivate;
};

#endif
