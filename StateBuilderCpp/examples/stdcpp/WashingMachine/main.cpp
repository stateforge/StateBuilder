
#include <iostream>
#include <cstdlib>
#include <iostream>


#include "main.h"
#include "WashingMachine.h"

using namespace std;


int main(int, char **)
{
    App app;
    return app.Main();
}


App::App()
{
}

int App::Main()
{
    cout << "WashingMachine" << endl;
    try {

        WashingMachine washingMachine;
        cout << "WashingMachine has been created" << endl;
        cout << "Send Start" << endl;
        washingMachine.Start();
        cout << "Send WashingDone" << endl;
        washingMachine.WashingDone();
        cout << "Send Fault" << endl;
        washingMachine.Fault();
        cout << "Send DiagnoseSuccess" << endl;
        washingMachine.DiagnoseSuccess();
        cout << "Send RinsingDone" << endl;
        washingMachine.RinsingDone();
        cout << "Send Fault" << endl;
        washingMachine.Fault();
        cout << "Send DiagnoseSuccess" << endl;
        washingMachine.DiagnoseSuccess();
        cout << "Send SpinningDone" << endl;
        washingMachine.SpinningDone();
        cout << "Done with events" << endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return -1;
    }
    cout << "WashingMachine should have been destroyed by this time" << endl;
    return 0;
}
