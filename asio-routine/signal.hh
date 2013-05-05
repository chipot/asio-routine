#pragma once
#ifndef SIGNAL_79QUWFXV
#define SIGNAL_79QUWFXV

#include <sstream>

#include <boost/asio/signal_set.hpp>

#include "asio-routine/asiofwd.hh"
#include "asio-routine/future.hh"
#include "asio-routine/coroutine_object.hh"
#include "asio-routine/print.hh"

#define signal_handler(error, signum) (boost::system::error_code const& error,\
                                       int signum)

namespace tnet {

class signal
        : public coroutine_object
{
  asio::signal_set _sigset;
public:
  template <class ...SIGNALS>
  signal(tnet::coroutine_object& parent, SIGNALS... sigs)
    : coroutine_object{parent, sprintfmt("signals: [%]", sprint(::strsignal(sigs)...))}
    , _sigset{parent.service(), sigs...}
  {}

  int
  wait();

  future<int>
  async_wait();
};

} /* tnet */

#endif /* end of include guard: SIGNAL_79QUWFXV */
