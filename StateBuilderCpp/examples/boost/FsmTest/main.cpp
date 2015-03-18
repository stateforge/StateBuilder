
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <logging.hpp>
#include "main.h"
 
using namespace std;
namespace bl = boost::logging;

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
  BOOST_LOG_L3("FsmTest begins"); 
  try {
    boost::asio::io_service io_service;
    FsmTest fsmTest(io_service);
    io_service.run();
  }
  catch (std::exception& e){
    BOOST_LOG_L1(string("Exception: ") + e.what());
    return -1;
  }
  BOOST_LOG_L3("FsmTest ends"); 
  return 0;
}


void App::logInit()
{
  BOOST_LOG_INIT(("" >> (*new bl::time_element("%H:%M:%S%F")) >> ","
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