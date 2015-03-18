#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <Acs.h>
#include <AcsRequestGetValue.h>
#include <AcsClient.h>

class GetValueApp : public QObject {

Q_OBJECT 

public:
  GetValueApp(const QString &host, const QString &paramName);

public slots:
   void requestEnd(bool error);

private:
  Acs m_acs;
  AcsClient m_client;
  AcsRequestGetValue m_requestGetValue;
};
