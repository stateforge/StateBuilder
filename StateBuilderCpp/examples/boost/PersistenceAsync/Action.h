#ifndef PERSISTENCEASYNC_H
#define PERSISTENCEASYNC_H

#include <cstdlib>
#include <string>
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

class Action {
public:
    Action();
    virtual ~Action();

    void Print(const std::string &message);

  private:
};

#endif
