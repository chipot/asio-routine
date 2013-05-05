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
#ifndef SCOPE_R1YECDZ0
#define SCOPE_R1YECDZ0

#include <string>
#include <vector>

#include "asio-routine/coroutine_object.hh"
#include "asio-routine/thread.hh"

namespace tnet {

class scope
{
 private:
  std::vector<tnet::thread *> _threads;
  tnet::coroutine_object& _root;
 public:
  scope(coroutine_object& root);
  ~scope();

  void
  run_background(std::string const&name, thread::action_t const &f);
  void
  run_background(thread::action_t const &f);
};

} /* tnet */


#endif /* end of include guard: SCOPE_R1YECDZ0 */
