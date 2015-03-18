#ifndef ACSMESSAGEPARAM_H
#define ACSMESSAGEPARAM_H

#include <QtCore/QString>
#include <QtCore/QList>

/**
  @class AcsParam
*/

class AcsParam
{
  public:
    AcsParam(const QString &name_, const QString &value_) : name(name_), value(value_) {};
    QString name;
    QString value;
};

typedef QList<AcsParam> AcsParamList;

/**
  @class  AcsParamDownload
*/

class AcsParamDownload
{
public:
  QString firmwareFilename;
  QString url;
  QString targetFilename;
  QString username;
  QString password;
};


#endif
