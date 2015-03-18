
#include <iostream>
#include <cstdlib>

#include "main.h"
#include "Samek.h"

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
  cout << "Samek" << endl; 
  try {
    Samek samek;
	samek.a();
	samek.b();
	samek.c();
	samek.d();
	samek.e();
	samek.f();
	samek.g();
	samek.h();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
	return -1;
  }
  return 0;
}
