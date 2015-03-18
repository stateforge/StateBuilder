#include <iostream>
#include "Action.h"

using namespace std;
using namespace carefusion;

// Led implementation
Action::Action(int iActionMax)
: _actionCount(0)
, _actionCountMax(iActionMax)
{
    
}

Action::~Action()
{
    
}

void Action::On()
{
  _actionCount++;
}

void Action::Off()
{
  _actionCount++;
}

bool Action::HasReachedMaxActions()
{
  return (_actionCount >= _actionCountMax);
}

