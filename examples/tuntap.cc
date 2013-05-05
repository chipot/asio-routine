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

#include <boost/asio/io_service.hpp>

#include <bindings/cpp/tuntap++.hh>

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <exception>

void
Main(tnet::coroutine_object& root)
{
  tuntap::tap tap;
  tap.ip("10.0.0.24", 24);
  tap.up();
  tnet::descriptor fd{root, tap.native_handle()};

  while (1)
  {
    std::vector<char> b(tap.mtu());
    size_t bytes = fd.read(asio::buffer(b));
    std::cout << "tap bytes read:" << bytes << std::endl;
  }
  tap.down();
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
