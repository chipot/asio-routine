#pragma once
#ifndef SESSION_R67EXL9L
#define SESSION_R67EXL9L

#include "asio-routine/asiofwd.hh"
#include "asio-routine/future.hh"
#include "asio-routine/coroutine_object.hh"

#include <boost/coroutine/coroutine.hpp>
#include <boost/asio/ip/tcp.hpp>


#define recv_handler(error, size) (const boost::system::error_code& error, \
                                   std::size_t size)

#define send_handler(error, size) (const boost::system::error_code& error, \
                                   std::size_t size)

namespace tnet {

class session
    : public coroutine_object
{
  tcp::socket       _socket;

 public:
  size_t
  read(asio::mutable_buffer b);

  size_t
  send(asio::const_buffer b);

  future<size_t>
  async_read(asio::mutable_buffer b);

  future<size_t>
  async_send(asio::const_buffer b);

  session(coroutine_object& parent, tcp::socket socket);

  ~session();
};

} /* tnet */

#endif /* end of include guard: SESSION_R67EXL9L */
