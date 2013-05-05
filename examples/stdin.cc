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

#include "asio-routine/thread.hh"
#include "asio-routine/utility.hh"
#include "asio-routine/descriptor.hh"
#include "asio-routine/signal.hh"
#include "asio-routine/print.hh"

#include <boost/asio/io_service.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <exception>

#include <cstdio>

void
Main(tnet::coroutine_object& root)
{
 tnet::descriptor _stdout{root, ::fileno(stdout)};
 tnet::descriptor _stdin{root, ::fileno(stdin)};

  while (1)
  {
    std::vector<char> b(1500);
    size_t bytes = _stdin.read(asio::buffer(b));
    b.resize(bytes);
    _stdout.write(asio::buffer(b));
  }
}

int
main(int argc, const char *argv[])
{
  asio::io_service io;
  tnet::coroutine_object prog(io);

  auto sig = [&] (tnet::coroutine_object& root)
  {
    tnet::signal sig(root, SIGINT);
    sig.wait();
    io.stop();
  };
  tnet::thread sigt(prog, "sigint", std::move(sig));

  auto main_ = [&] (tnet::coroutine_object& p)
  {
    try
    {
      Main(p);
    }
    // Thinking about a IOError here..
    catch (std::runtime_error const& e)
    {
      print(e.what()); // useful for EOF
      io.stop();
    }
  };
  tnet::thread t(prog, "echo", std::move(main_));

  io.run();
  print("stopped.");
  return (0);
}
