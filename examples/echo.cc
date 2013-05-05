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
#include "asio-routine/thread.hh"
#include "asio-routine/scope.hh"
#include "asio-routine/utility.hh"

#include <boost/asio/io_service.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <exception>

void
Main(tnet::coroutine_object& root)
{
  tnet::server serv(root, {tcp::v4(), 9999});
  tnet::scope scope(root);

  while (1)
  {
    std::move_on_copy<tcp::socket> sock{serv.async_accept()};

    std::string name = std::to_string(sock.value);
    auto sock_fn = [&, sock] (tnet::coroutine_object& p)
    {
      tnet::session session(p, std::move(sock.value));

      try
      {
        while (1)
        {
          std::vector<char> b(512, '\0');
          size_t size_read = session.read(asio::buffer(b));
          b.resize(size_read);
          asio::const_buffer cb = asio::buffer(b);
          session.send(cb);
        }
      }
      catch (std::exception const &e)
      {
        std::cout << "native exception: " << e.what() << std::endl;
      }
      print("Terminating session.");
    };
    scope.run_background(name, sock_fn);
    std::cout << "accepted" << std::endl;
  }
}

int
main(int argc, const char *argv[])
{
    asio::io_service io;

    tnet::coroutine_object prog(io);
    auto main_ = [&] (tnet::coroutine_object& p)
    {
      Main(p);
    };
    tnet::thread t(prog, "echo", std::move(main_));

    std::cout << "run !" << std::endl;
    io.run();
    return (0);
}
