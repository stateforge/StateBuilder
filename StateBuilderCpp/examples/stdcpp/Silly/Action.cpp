#include <iostream>
#include <ObserverStd.h>
#include "Action.h"

using namespace std;



void Action::printEntryHigh()
{
    cout << "printEntryHigh" << endl;
}

void Action::printExitHigh()
{
    cout << "printExitHigh" << endl;
}

void Action::printEntryLow()
{
    cout << "printEntryLow" << endl;
}

void Action::printExitLow()
{
    cout << "printExitLow" << endl;
}

void Action::printAlreadyHigh()
{
    cout << "printAlreadyHigh" << endl;
}

void Action::printAlreadyLow()
{
    cout << "printAlreadyLow" << endl;
}

void Action::changeColor(int color)
{
    cout << "changeColor " << color << endl;
}