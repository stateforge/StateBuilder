
#include <iostream>
#include <cstdlib>
#include <iostream>

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
    int iTries = 1000;
    #define NUM_EVENT_FEED 10
    //boost::timer timer;
    BOOST_LOG_L3("Microwave");
    try {

        Microwave microwave;
        BOOST_LOG_L3("Microwave has been created");
        BOOST_LOG_L3("Send evStart")
        microwave.evStart();

        for(int i = 0; i < iTries; i++){
            feedFsm(microwave);
        }
        BOOST_LOG_L3("Send evStop")
        microwave.evStop();
    }
    catch (std::exception& e)
    {
        BOOST_LOG_L1(string("Exception: ") + e.what());
        return -1;
    }
    BOOST_LOG_L3("Microwave ends");
    //cout << "Elapsed time " << timer.elapsed() << " seconds" << endl;
    return 0;
}

void App::feedFsm(Microwave &microwave)
{
    microwave.evDoorOpened();
    microwave.evTurnOn();
    microwave.evDoorClosed();
    microwave.evDoorClosed();
    microwave.evCookingDone();
    microwave.evDoorOpened();
    microwave.evTurnOff();
    microwave.evDoorOpened();
    microwave.evTurnOn();
    microwave.evDoorClosed();
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