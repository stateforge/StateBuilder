#ifndef ACSMESSAGEBUILDER_H
#define ACSMESSAGEBUILDER_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <AcsMessageParam.h>
#include <Tr69Tag.h>

class QtSoapMessage;
class QHttpResponseHeader;

/**
  @class AcsMessageBuilder
*/
class AcsMessageBuilder : public QObject, private Tr69Tag
{
    Q_OBJECT

  public:

    AcsMessageBuilder(QObject *pParent = NULL);
    ~AcsMessageBuilder();

    void buildEnvelope(QtSoapMessage &message);
    void buildHttpHeaderResponse(QHttpResponseHeader &responseHeader, 
                                 int iContentLength, 
                                 int code = 200, 
                                 const QString& status = "OK");

    void buildRebootRequest(QtSoapMessage &request);
    void buildDownloadRequest(QtSoapMessage &request, AcsParamDownload &paramDownload);
    void buildInformResponse(const QString &id, QtSoapMessage &informResponse);
    void buildGetParameterNamesRequest(QtSoapMessage &request);
    void buildSetValueRequest(QtSoapMessage &request, AcsParamList &paramList);
    void buildGetValueRequest(QtSoapMessage &request, AcsParamList &paramList);
    void buildTransferCompleteResponse(QtSoapMessage &response);

    const QString& nsPrefix() const {return m_nsPrefix;};

  private:
    QString m_id;
    QString m_nsPrefix;
};

#endif
