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
#ifndef DESCRIPTOR_T7SHL0RR
#define DESCRIPTOR_T7SHL0RR

#include "asio-routine/asiofwd.hh"
#include "asio-routine/coroutine_object.hh"
#include "asio-routine/future.hh"

#include <boost/asio/posix/stream_descriptor.hpp>

namespace tnet {

class descriptor
    : public coroutine_object
{
private:
    asio::posix::stream_descriptor _fd;
 public:
    descriptor(tnet::coroutine_object& parent, int fd);
    ~descriptor();

    size_t
    write(asio::const_buffer b);

    size_t
    read(asio::mutable_buffers_1 b);

    future<size_t>
    async_write(asio::const_buffer b);

    future<size_t>
    async_read(asio::mutable_buffers_1 b);
};

} /* tnet */

#endif /* end of include guard: DESCRIPTOR_T7SHL0RR */
