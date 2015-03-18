
#include "LoginMvc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool use_gmail = QSslSocket::supportsSsl () && !a.arguments().contains("--dummy");

    LoginMvc loginMvc(use_gmail);
    loginMvc.show();
    //Kick the fsm
    loginMvc.start();
    return a.exec();
}

