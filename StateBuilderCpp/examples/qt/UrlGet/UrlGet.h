#ifndef UrlGet_H
#define UrlGet_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

class QTimer;
class QHttp;
class QFtp;
class UrlGetContext; // Generated class

class UrlGet : public QObject
{
    Q_OBJECT
    
public:
    UrlGet();
    virtual ~UrlGet();

    void httpStart();
    void ftpStart();
    
    QHttp* getHttp();
    QFtp* getFtp();

public slots:
    void start();
  
private slots:

    void contextEnd();

    void httpDone(bool error);
    void httpStateChange(int state);
    
    void ftpDone(bool error);
    void ftpStateChange(int state);
private:

    UrlGetContext *m_pUrlGetContext; // Generated class

    void logInit();

    //Http
    QHttp *m_pHttp;
    QString m_httpHost;
    QString m_httpFile;
    void httpInit();
    
    //Ftp
    QFtp *m_pFtp;
    QString m_ftpHost;
    QString m_ftpFile;
    void ftpInit();
    
};

#endif // UrlGet_H
