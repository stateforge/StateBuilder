#ifndef ACS_H
#define ACS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

class AcsPrivate;
class AcsParamDownload;

typedef QString AcsClientToken;
typedef QString RequestToken;

/**
  @class Acs
*/
class Acs : public QObject
{
    Q_OBJECT
    

  public:
    Acs();
    ~Acs();

    bool listen();
    bool clientCreate(AcsClientToken &clientToken, const QUrl &target);
    bool clientDestroy(AcsClientToken &clientToken);
    bool clientStart(const AcsClientToken &clientToken);
    const QString& clientGetError(const AcsClientToken &clientToken);
    
    void addRequestReboot(const AcsClientToken &clientToken);
    void addRequestDownload(const AcsClientToken &clientToken, AcsParamDownload &paramDownload);

  signals:
    void clientEnd(AcsClientToken &clientToken, bool error);
    void clientNew(const AcsClientToken &clientToken);
    void clientTokenInvalid(const AcsClientToken &clientToken);

  private:
    friend class AcsPrivate;
    AcsPrivate *m_pPrivate;
};

#endif
