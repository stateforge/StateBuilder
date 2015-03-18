#ifndef SAMEK_H
#define SAMEK_H

#include <cstdlib>
#include <iostream>
#include <string>
#include "SamekFsm.h"

using namespace std;

class Samek
{

public:
    Samek();
    ~Samek();

    void a();
    void b();
    void c();
    void d();
    void e();
    void f();
    void g();
    void h();

    int foo;
    void Print(const std::string &message);

private:
    SamekContext& GetContext();
    SamekContext *m_pContext;

};

#endif
