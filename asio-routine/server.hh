#pragma once
#ifndef SERVER_6DJWQFK7
#define SERVER_6DJWQFK7

#include <boost/coroutine/coroutine.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "asio-routine/asiofwd.hh"
#include "asio-routine/coroutine_object.hh"
#include "asio-routine/future.hh"

namespace tnet {

class server
    : public coroutine_object
{
 private:
  tcp::acceptor       _accp;

 public:
  server(coroutine_object& parent, tcp::endpoint const&);

  future<tcp::socket>
  async_accept();

  tcp::socket
  accept();
};

} /* tnet */


#endif /* end of include guard: SERVER_6DJWQFK7 */
