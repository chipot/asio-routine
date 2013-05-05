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

#pragma once
#ifndef THREAD_JB2HEGUM
#define THREAD_JB2HEGUM

#include "asio-routine/coroutine_function.hh"
#include "asio-routine/asiofwd.hh"
#include "asio-routine/print.hh"

#include <boost/asio/io_service.hpp>

namespace tnet {

template <typename T = void>
class vthread
    : public coroutine_function
{
 public:
  using action_t = std::function<T (coroutine_function &)>;

  vthread(coroutine_object& parent,
          action_t f)
    : vthread{parent, "", f}
  {
  }
  ~vthread() = default;

  vthread(coroutine_object& parent,
          std::string const& name,
          action_t f)
    : coroutine_function(parent, name)
  {
    auto fn = [&, f]
    {
      auto _fn = [&, f] {
        f(*this);
      };
      this->start(std::move(_fn));
    };
    this->service().post(std::move(fn));
  }
};

using thread = vthread<>;

} /* tnet */

#endif /* end of include guard: THREAD_JB2HEGUM */
