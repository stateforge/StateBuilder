
#include <iostream>
#include <cstdlib>
#include <sstream> // for serialization
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

#include <logging.hpp>
#include <SmBoostObserver.h>

#include "main.h"
#include "Action.h"
#include "PersistenceAsyncFsm.h"

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

  BOOST_LOG_L3("PersistenceAsync");

  try {
    std::stringstream ios;
    string serializedContent;
    boost::asio::io_service io_service;
    size_t eventsProcessed;
    boost::system::error_code errorCode;
    Action action;

    //Create the state machine context and set the observer 
    PersistenceAsyncContext *pContext = new PersistenceAsyncContext(action, io_service);
    pContext->SetObserver(SmBoostObserver::GetInstance());

    //Serialize after creation
    ios << *pContext;
	BOOST_LOG_L3("Serialize after creation " << endl << ios.str());
    ios.str("");

    // EnterInitialState will execue the on entry chain from the root state to the initial state
    pContext->EnterInitialState();
    ios << *pContext;
	BOOST_LOG_L3("Serialize after EnterInitialState " << endl << ios.str());
    ios.str("");

    // Send the event start asynchronously. 
    pContext->EvStart();
    // Now that the event start is in the queue, process it 
    eventsProcessed = io_service.run_one(errorCode);

    BOOST_LOG_L3("error code " << errorCode);
    assert(eventsProcessed == 1);
    // reset to allow the next  run_one() to work
    io_service.reset();

    ios << *pContext;
    BOOST_LOG_L3("Serialize after EvStart " << endl << ios.str());
    ios.str("");

    pContext->Event2();
    ios << *pContext;
    BOOST_LOG_L3("Serialize after Event2 " << endl << ios.str());
    ios.str("");

    pContext->Event3();
    ios << *pContext;
    serializedContent = ios.str();
    BOOST_LOG_L3("Serialize after Event3 " << endl << ios.str());

    delete pContext;
    // WARNING io_service cannot be used after the context is deleted when some events are still in the queue
    boost::asio::io_service io_serviceNew;
    //Create a new context
    pContext = new PersistenceAsyncContext(action, io_serviceNew);
    pContext->SetObserver(SmBoostObserver::GetInstance());
    //Unserialize
    ios >> *pContext;
    ios.str("");

    //Serialize and compare result
    ios << *pContext;
	BOOST_LOG_L3("Serialize after Unserialize " << endl << ios.str());
    assert(serializedContent == ios.str());
    ios.str("");

    //Process the event 2 and 3
    eventsProcessed = io_serviceNew.run_one(errorCode);
    BOOST_LOG_L3("error code " << errorCode);
    // whatever the number of event in the queue, run_one() returns 1.
    assert(eventsProcessed == 1);
    io_serviceNew.reset();

    ios << *pContext;
    BOOST_LOG_L3("Serialize after processsing " << endl << ios.str());
    ios.str("");

    // Send the event stop
    pContext->EvStop();
    //The EvStop will trigger the state machine into the final state
    //io_service.run() normally block untill the state machine enters the final state.
    eventsProcessed = io_serviceNew.run(errorCode);
    assert(eventsProcessed == 1);

    ios << *pContext;
    BOOST_LOG_L3("Serialize after EvStop " << endl << ios.str());
    ios.str("");

    delete pContext;
  }
  catch (std::exception& e)
  {
    BOOST_LOG_L1(string("Exception: ") + e.what());
    iRc = -1;
  }

  BOOST_LOG_L3("PersistenceAsync ends");
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
  bl::sink sinkStdOutput(&std::cout, BOOST_LOG_MASK_LEVEL_4);
  sinkStdOutput.attach_qualifier(bl::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sinkStdOutput);

  //File
  bl::sink sinkFile(new std::ofstream("./log.txt"), BOOST_LOG_MASK_LEVEL_4);
  sinkFile.attach_qualifier(bl::log);
  BOOST_LOG_ADD_OUTPUT_STREAM(sinkFile);
}
