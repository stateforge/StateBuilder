#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <cstdlib>
#include <string>
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

class HelloWorldContext;

class HelloWorld
{
public:
    HelloWorld(boost::asio::io_service &io_service);
    virtual ~HelloWorld();
    
    void IncrementCounter(){iCounter++;};
    bool HasReachedLimit();
    void Print(const std::string& message);
    
  private:
    HelloWorldContext *m_pContext; //Generated class
    int iRtxCount;
    int iCounter;
};

#endif
