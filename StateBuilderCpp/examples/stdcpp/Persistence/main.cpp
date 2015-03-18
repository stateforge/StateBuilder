#include <iostream>
#include <cstdlib>
#include <sstream> // for serialization
#include <ObserverStd.h>
#include "Persistence.h"
#include "Persistence01Fsm.h" // Generated file
#include "PersistenceParallel01Fsm.h" // Generated file
#include "PersistenceParallel02Fsm.h" // Generated file

using namespace std;

void test1()
{
    std::cout << "test1: " << std::endl;
	// Setup 
	std::stringstream ios;
	Action *pAction = new Action();
	Persistence01Context *pContext = new Persistence01Context(*pAction);
    pContext->SetObserver(fsm::ObserverStd::GetInstance());
	//Serialize to ios
	ios << *pContext;
	cout << "Serialize after init " << ios.str() << endl;

	// send evStart
	pContext->evStart();
	cout << "Serialize after evStart " << *pContext << endl;

	// send evStop
    pContext->evStop();
    ios << *pContext;
	cout << "Serialize after evStop " << *pContext << endl;

	delete pContext;
	delete pAction;
  
}

void test2()
{
    std::cout << "test2: " <<endl;
	std::stringstream ios;
	Action action;
	Persistence01Context *pContext = new Persistence01Context(action);
    pContext->SetObserver(fsm::ObserverStd::GetInstance());
	pContext->evStart();

	ios << *pContext;
	cout << "Serialize: " << endl << ios.str() << endl;
   
	delete pContext;

	Persistence01Context *pContextRestored = new Persistence01Context(action);
    pContextRestored->SetObserver(fsm::ObserverStd::GetInstance());
	ios >> *pContextRestored;
	cout << "Unserialize: " << endl << *pContextRestored << endl;
	delete pContextRestored;
}

void testPersistenceParallel01()
{
    std::cout << "testPersistenceParallel01: " <<endl;
	std::stringstream ios;
	Action action;
	PersistenceParallel01Context *pContext = new PersistenceParallel01Context(action);
    pContext->SetObserver(fsm::ObserverStd::GetInstance());
	pContext->evStart();

	ios << *pContext;
    std::cout << "Serialize: " << endl << ios.str() << endl;

	delete pContext;

	PersistenceParallel01Context *pContextRestored = new PersistenceParallel01Context(action);
    pContextRestored->SetObserver(fsm::ObserverStd::GetInstance());
	ios >> *pContextRestored;
	cout << "Unserialize: " << endl << *pContextRestored << endl;
	delete pContextRestored;
}

// state machine with first state as parallel 
void testParallel02()
{
    std::cout << "testParallel02: " <<endl;
	std::stringstream ios;
	Action action;
	PersistenceParallel02Context *pContext = new PersistenceParallel02Context(action);
    //EnterInitialState is compulsory for state machine with first state as parallel 
    pContext->EnterInitialState();
    pContext->SetObserver(fsm::ObserverStd::GetInstance());

    ios << *pContext;
    cout << "Serialize Creation " << endl << ios.str() << endl;
    assert(ios.str().find("Running") != string::npos);
    assert(ios.str().find("A1") != string::npos);
    assert(ios.str().find("B1") != string::npos);
    ios.str("");

    //Ev12
	pContext->Ev12();

    ios << *pContext;
    cout << "Serialize Ev12 " << endl << ios.str() << endl;
    assert(ios.str().find("Running") != string::npos);
    assert(ios.str().find("A2") != string::npos);
    assert(ios.str().find("B2") != string::npos);
    ios.str("");

    //Ev23
    pContext->Ev23();
    ios << *pContext;
    cout << "Serialize Ev23 " << endl << ios.str() << endl;
    assert(ios.str().find("Idle") != string::npos);
    ios.str("");

    //EvRunning
    pContext->EvRunning();
    ios << *pContext;
    cout << "Serialize EvRunning " << endl << ios.str() << endl;
    ios.str("");

    //EvStop
    pContext->EvStop();
	ios << *pContext;
    std::cout << "Serialize: " << endl << ios.str() << endl;
    assert(ios.str().find("End") != string::npos);
    ios.str("");
	delete pContext;

}
int main(int /*argc*/, char ** /*argv[]*/)
{
  std::cout << "PersistenceSync" << std::endl;
  int iRv = 0;
  try {
    testParallel02();
    testPersistenceParallel01();
    test1();
    test2();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    iRv =  -1;
  }

  return iRv;;
}




