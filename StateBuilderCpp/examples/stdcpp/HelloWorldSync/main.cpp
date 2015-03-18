#include <iostream>
#include <cstdlib>

#include "HelloWorld.h"

using namespace std;

int main(int /*argc*/, char ** /*argv[]*/)
{
  cout << "HelloWorldSync" << endl;
  int iRv = 0;
  try {
    HelloWorld helloWorld;
    helloWorld.start();
    helloWorld.stop();
  }
  catch (std::exception& e)
  {
    cerr << "Exception: " << e.what() << "\n";
    iRv =  -1;
  }

  return iRv;;
}

