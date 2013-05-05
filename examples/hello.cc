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

 std::string hello = "hello, ", world = "world\n";
 {
   auto world_promise = _stdout.async_write(asio::buffer(world));
   auto hello_promise = _stdout.async_write(asio::buffer(hello));
   size_t hello_size = hello_promise.get(); // get member function
   size_t world_size = world_promise; // cast operator
   printfmt("msg is % bytes\n", hello_size + world_size);
 }
 {
   tnet::future<size_t> world_promise = _stdout.async_write(asio::buffer(world));
   tnet::future<size_t> hello_promise = _stdout.async_write(asio::buffer(hello));
   // As expected, we can add size_t through their future.
   // Don't forget that the evaluation order is undefined.
   printfmt("msg is % bytes\n", hello_promise + world_promise);
 }
}

int
main(int argc, const char *argv[])
{
  asio::io_service io;
  tnet::coroutine_object prog(io);

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
  tnet::thread t(prog, "hello", std::move(main_));

  io.run();
  return (0);
}
