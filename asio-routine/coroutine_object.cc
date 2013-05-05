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

#include "asio-routine/coroutine_object.hh"
#include "asio-routine/coroutine_function.hh"

namespace tnet {

coroutine_object::coroutine_object(coroutine_object& parent,
                                   std::string const& name) 
  : _io{parent.service()}
  , _name{name}
  , _parent{&parent}
{
  if (auto ptr = dynamic_cast<coroutine_function*>(&parent))
  {
    ptr->_children.push_back(this);
  }
}

coroutine_object::coroutine_object(asio::io_service &service,
                                   std::string const& name)
  : _io{service}
  , _name{name}
  , _parent{nullptr}
{
}

asio::io_service&
coroutine_object::service()
{
  return _io;
}

void
coroutine_object::yield()
{
  this->_parent->yield();
}

void
coroutine_object::cont()
{
  this->_parent->cont();
}

std::string const&
coroutine_object::name()
{
  return _name;
}

} /* tnet */
