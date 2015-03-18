
#include <iostream>
#include <cstdlib>
#include <iostream>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <logging.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include "main.h"
#include "Door.h"
#include "DoorTester.h"

using namespace std;
namespace bl = boost::logging;

int main(int /*argc*/, char ** /*argv[]*/)
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
    BOOST_LOG_L3("Door Example");

    try {
        boost::asio::io_service io_service;
        BOOST_LOG_L3("Door constructor ");
        Door door(io_service);
        BOOST_LOG_L3("DoorTester constructor ");
        DoorTester doorTester(door, io_service);
        BOOST_LOG_L3("Send the start event");
        doorTester.EvStartTest();
        BOOST_LOG_L3("run the main loop");
        io_service.run();
        BOOST_LOG_L3("End of run");
    }
    catch (std::exception& e){
        BOOST_LOG_L3("Exception: " << e.what());
        return -1;
    }
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