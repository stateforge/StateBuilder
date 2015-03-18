#include <iostream>
#include <cstdlib>
#include "Player.h"
#include <ev.h>
#include <sstream>

using namespace std;

int main(int /*argc*/, char ** /*argv[]*/)
{
  int iReturnValue = 0;
  cout << "Player" << endl; 
  try {
    ev::default_loop    evLoop;
    Player player(evLoop);
    player.On();
    evLoop.run(0);
    
    stringstream ios;
    ios << player;
    // 
    cout << "Serialize to" << endl;
    cout << ios.str() << endl;
    //player.On();
    Player playerNew(evLoop);
    ios >> playerNew;
    
    playerNew.Off();
    evLoop.run(0);
    
    //ios << playerNew;
    cout << "Serialize to" << endl;
    cout << playerNew << endl;
    
    
  }
  catch (std::exception& e){
    cerr << "Exception: " << e.what() << endl;
    iReturnValue = -1;
  }
  return iReturnValue;
}
