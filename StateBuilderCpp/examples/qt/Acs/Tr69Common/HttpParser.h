#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>

#include <QtCore/QObject>
#include <QtCore/QLinkedList>

#include <QtNetwork/QAbstractSocket>

class QTcpServer;
class QTcpSocket;
class QHttp;
class QAuthenticator;

class HttpParser
{
  public:
    HttpParser();
    ~HttpParser();

    enum status_t {
      Done,
      Error,
      Incomplete
    };

    status_t addBytes(QByteArray &bytes);

    const char* getMethod();
    const char* getUri();
    const char* getQueryString();
    const char* getBody();
    // key should be in lower case when looking up.
    const char* getValue( const char* key );
    unsigned getContentLength();

  private:
    void parseHeader();
    bool parseRequestLine();

    QByteArray _data;
    unsigned _headerStart;
    unsigned _bodyStart;
    unsigned _parsedTo;
    unsigned _keyIndex;
    unsigned _valueIndex;
    unsigned _contentLength;
    unsigned _contentStart;
    unsigned _uriIndex;

    typedef std::vector<unsigned> IntArray;
    IntArray _keys;

    int _state;

    enum State {
      p_request_line = 0,
      p_request_line_cr = 1,
      p_request_line_crlf = 2,
      p_request_line_crlfcr = 3,
      p_key = 4,
      p_key_colon = 5,
      p_key_colon_sp = 6,
      p_value = 7,
      p_value_cr = 8,
      p_value_crlf = 9,
      p_value_crlfcr = 10,
      p_content = 11, // here we are done parsing the header.
      p_error = 12 // here an error has occurred and the parse failed.
    };

    status_t _status;
};

#endif
