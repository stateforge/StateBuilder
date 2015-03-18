
#include <iostream>
#include <cstdlib>
#include <iostream>

#include "main.h"
#include "Light.h"

using namespace std;

int main(int /*argc*/, char ** /*argv[]*/)
{
  App app;
  return app.Main();
}

App::App()
{
}

int App::Main()
{
  cout << "Light" << endl; 
  try {
    Light light;
    light.on();
    light.off();
  }
  catch (std::exception& e)
  {
    cerr << "Exception: " << e.what() << endl;
    return -1;
  }
  return 0;
}
