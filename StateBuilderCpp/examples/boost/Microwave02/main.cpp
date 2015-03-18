
#include <iostream>
#include <cstdlib>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <logging.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include "main.h"
#include "Microwave.h"

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
  BOOST_LOG_L3("Microwave02");

    try {
        int duration = 1; /* seconds */
        Microwave microwave;
        BOOST_LOG_L3("Microwave02 has been created");
        BOOST_LOG_L3("Send evStart");
        microwave.evStart();
        BOOST_LOG_L3("Start the worker threads");
        microwave.workersStart();
        BOOST_LOG_L3("Sleep main thread for " << duration << " seconds");
        boost::this_thread::sleep(boost::posix_time::seconds(duration));
        BOOST_LOG_L3("Send evStop");
        microwave.evStop();
        BOOST_LOG_L3("Stop the worker threads");
        microwave.workersStop();
    }
    catch (std::exception& e)
    {
        BOOST_LOG_L1("Exception: " << e.what());
        return -1;
    }
    BOOST_LOG_L3("Microwave02 should have been destroyed by this time");
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
