#ifndef TR69MESSAGEBUILDER_H
#define TR69MESSAGEBUILDER_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <Tr69MessageParam.h>


class QtSoapMessage;

/**
  @class Tr69MessageBuilder
*/
class Tr69MessageBuilder : public QObject
{
    Q_OBJECT

  public:

    Tr69MessageBuilder(QObject *pParent);
    ~Tr69MessageBuilder();

    void buildEnvelope(QtSoapMessage &message);

    void buildInformRequest(QtSoapMessage &informRequest);

    const QString& nsPrefix() const {return m_nsPrefix;};

  private:
    QString m_id;
    QString m_nsPrefix;
};

#endif
