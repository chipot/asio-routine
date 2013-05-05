/**
 * Copyright (c) 2012, PICHOT Fabien Paul Leonard <pichot.fabien@gmail.com>
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/

#include "asio-routine/server.hh"
#include "asio-routine/session.hh"
#include "asio-routine/future.hh"

#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/none.hpp>

#define accept_handler(NAME) (boost::system::error_code NAME)

namespace tnet {

std::string
to_string(tcp::endpoint const& endpoint)
{
  std::stringstream ss;
  ss << endpoint;
  return ss.str();
}

server::server(coroutine_object& parent, tcp::endpoint const& endpoint)
    : coroutine_object{parent, "serv " + to_string(endpoint)}
    , _accp{this->service(), endpoint}
{
}

future<tcp::socket>
server::async_accept()
{
    auto task = [&] {
      return this->accept();
    };
    return future<tcp::socket>(task);
}

tcp::socket
server::accept()
{
   boost::system::error_code error;
   tcp::socket               sock{this->service()};

   this->_accp.async_accept(sock, [&] accept_handler(E)
   {
     error = E;
     this->cont();
   });
   yield();
   if (error)
       throw std::runtime_error{error.message()};
   return sock;
}

} /* tnet */
