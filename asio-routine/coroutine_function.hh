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
#ifndef COROUTINE_FUNCTION_YQ7SQPIW
#define COROUTINE_FUNCTION_YQ7SQPIW

#include "asio-routine/coroutine_object.hh"
#include <memory>

namespace tnet {
  
class coroutine_function: public coroutine_object
{
 private:
  using coro_t = typename boost::coroutines::coroutine<void ()>;
  using caller_type = typename coro_t::caller_type;
  std::vector<coroutine_object*> mutable _children;

  friend class coroutine_object;

  coro_t* _coro;
  caller_type* _ca = nullptr;
 public:
  coroutine_function(asio::io_service& io);

  coroutine_function(coroutine_object &parent, std::string const& name);

  ~coroutine_function() = default;

  void yield();

  void cont();

  void
  show_children(int indent = 0);

  template <class T>
  void start(T const &f)
  {
    auto coro = [&, f] (caller_type &ca)
    {
      this->_ca = &ca;
      f();
    };
    this->_coro = new coro_t(std::move(coro));
  }

};

} /* tnet */


#endif /* end of include guard: COROUTINE_FUNCTION_YQ7SQPIW */
