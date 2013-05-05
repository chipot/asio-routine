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

#include "asio-routine/session.hh"
#include "asio-routine/utility.hh"

#include <boost/asio/ip/tcp.hpp>

namespace tnet {
  
session::session(coroutine_object& parent, tcp::socket socket)
  : coroutine_object{parent, std::to_string(socket)}
  , _socket{std::move(socket)}
{
}

session::~session()
{
}

size_t
session::read(asio::mutable_buffer b)
{
  boost::system::error_code error;
  size_t size;

  auto cb_fn = [&] recv_handler(_error, _size)
  {
    if (_error)
    {
      error = _error;
    }
    size = _size;
    this->cont();
  };
  this->_socket.async_read_some(asio::buffer(b), cb_fn);
  yield();
  if (error)
  {
    throw std::runtime_error{error.message()};
  }
  return size;
}

size_t
session::send(asio::const_buffer b)
{
  boost::system::error_code error;
  size_t size;

  auto cb_fn = [&] send_handler(_error, _size)
  {
    if (_error)
    {
      error = _error;
    }
    size = _size;
    this->cont();
  };
  this->_socket.async_send(asio::buffer(b), cb_fn);
  yield();
  if (error)
      throw std::runtime_error{error.message()};
  return size;
}

future<size_t>
session::async_read(asio::mutable_buffer b)
{
  auto task = [&, b] {
    return this->read(b);
  };
  return future<size_t>(task);
}

future<size_t>
session::async_send(asio::const_buffer b)
{
  auto task = [&, b] {
    return this->send(b);
  };
  return future<size_t>(task);
}

} /* tnet */

