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

#include "asio-routine/signal.hh"
#include <unistd.h>

namespace tnet {

int
signal::wait()
{
  boost::system::error_code error;
  int signum = -1;

  auto sig_fn = [&] signal_handler(_error, _num)
  {
    error = _error;
    signum = _num;
    this->cont();
  };
  _sigset.async_wait(sig_fn);
  yield();
  if (error)
      throw std::runtime_error{error.message()};
  return signum;
}

future<int>
signal::async_wait()
{
  auto task = [&] {
    return this->wait();
  };
  return future<int>(task);
}
  
} /* tnet */
