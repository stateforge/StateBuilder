#ifndef TR69TAG_H
#define TR69TAG_H

#include <QtCore/QString>

/**
  @class AcsConnection
*/
class Tr69Tag
{

  public:
    Tr69Tag();
    virtual ~Tr69Tag();
 
    static const char* NS;
    static const char* NS_PREFIX;
    static const char* TAG_ID;

    static const char* TAG_STATUS;

    static const char* TAG_REBOOT;
    static const char* TAG_REBOOT_RESPONSE;
    static const char* TAG_COMMAND_KEY;

    static const char* TAG_DOWNLOAD;
    static const char* TAG_DOWNLOAD_RESPONSE;
    static const char* TAG_FILE_TYPE;
    static const char* TAG_FIRMWARE_UPGRADE_IMAGE;
    static const char* TAG_DOWNLOAD_ACS;
    static const char* TAG_URL;
    static const char* TAG_USERNAME;
    static const char* TAG_PASSWORD;
    static const char* TAG_TARGET_FILE_NAME;
    static const char* TAG_FILE_SIZE;

    static const char* TAG_TRANSFER_COMPLETE;
    static const char* TAG_TRANSFER_COMPLETE_RESPONSE;

    static const char* TAG_INFORM;
    static const char* INFORMRESPONSE;
    static const char* TAG_MAXENVELOPES;

    static const char* TAG_GETPARAMETERNAMES;
    static const char* TAG_GETPARAMETERNAMESRESPONSE;

    static const char* TAG_SETPARAMETERVALUES;
    static const char* TAG_SETPARAMETERVALUESRESPONSE;

    static const char* TAG_GETPARAMETERVALUES;
    static const char* TAG_GETPARAMETERVALUESRESPONSE;

    static const char* TAG_PARAMETERLIST;
    static const char* TAG_PARAMETERVALUESTRUCT;

    static const char* TAG_NAME;
    static const char* TAG_VALUE;

    static const char* TAG_PARAMETERNAMES;
    static const char* TAG_STRING;

    static const char* TAG_FAULTSTRUCT;
    static const char* TAG_FAULTSTRING;
    static const char* TAG_FAULTCODE;
};

#endif
