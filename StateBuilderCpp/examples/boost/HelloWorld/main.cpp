
#include <iostream>
#include <cstdlib>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <logging.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include "main.h"
#include "HelloWorld.h"

using namespace std;
namespace bl = boost::logging;
using namespace boost::posix_time;

int main(int, char **)
{
  App app;
  return app.Main();
}

App::App()
{
    logInit();
}

int App::Main()
{
  int iRc = 0;


  BOOST_LOG_L3("HelloWorld");
  
  try {
    boost::asio::io_service io_service;
    time_duration testDurationExpected = milliseconds(3000);
    time_duration testTolerance = milliseconds(500);
    
    HelloWorld helloWorld(io_service);
    ptime startTime = microsec_clock::local_time();
    BOOST_LOG_L3("Start Time : " << to_simple_string(startTime));
    io_service.run();
    ptime endTime = microsec_clock::local_time();
    time_duration testDuration = endTime - startTime;
    BOOST_LOG_L3("End Time : " << to_simple_string(startTime));
    BOOST_LOG_L3("Duration: " << to_simple_string(testDuration));
    if(testDuration + testTolerance < testDurationExpected){
        BOOST_LOG_L1("test runs in less time than expected");
        //assert(0);
        iRc = -1;
    } else if (testDurationExpected + testTolerance < testDuration){
        BOOST_LOG_L1("test runs in more time than expected");
        //assert(0);
        iRc = -1;
    } else {
        BOOST_LOG_L1("test runs within margin");
      //ok
    }
  }
  catch (std::exception& e)
  {
    BOOST_LOG_L1(string("Exception: ") + e.what());
    iRc = -1;
  }

  BOOST_LOG_L3("HelloWorld ends");
  return iRc;
}

void App::logInit()
{
  BOOST_LOG_INIT(("" >> (*new bl::time_element("%H:%M:%S.%f")) >> ","
                     >> bl::mask >> ","
                     >> (*new bl::pretty_function_name_element()) >> ":"
                     >> bl::line >> ":"
                     >> bl::trace
                     >> bl::eol)); // log format
  
  //Standard output
  bl::sink sinkStdOutput(&std::cout, BOOST_LOG_MASK_LEVEL_3);
  sinkStdOutput.attach_qualifier(bl::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sinkStdOutput);

  //File
  bl::sink sinkFile(new std::ofstream("./log.txt"), BOOST_LOG_MASK_LEVEL_4);
  sinkFile.attach_qualifier(bl::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sinkFile);
}
