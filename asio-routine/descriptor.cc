#include "asio-routine/descriptor.hh"
#include "asio-routine/print.hh"

#define recv_handler(error, size) (const boost::system::error_code& error, \
                                   std::size_t size)

#define send_handler(error, size) (const boost::system::error_code& error, \
                                   std::size_t size)

namespace tnet {

descriptor::descriptor(coroutine_object& parent, int fd)
  : coroutine_object{parent}
  , _fd{this->service(), fd}
{}

descriptor::~descriptor()
{
  this->_fd.release(); // Don't own the descriptor yet
}

size_t
descriptor::write(asio::const_buffer b)
{
  boost::system::error_code error;
  size_t size;

  auto send_fn = [&] send_handler(_error, _size)
  {
    if (_error)
      error = _error;
    size = _size;
    this->cont();
  };
  this->_fd.async_write_some(asio::buffer(b), send_fn);
  yield();
  if (error)
    throw std::runtime_error{error.message()};
  return size;
}

size_t
descriptor::read(asio::mutable_buffers_1 b)
{
  boost::system::error_code error;
  size_t size;

  auto recv_fn = [&] recv_handler(_error, _size)
  {
    if (_error)
      error = _error;
    size = _size;
    this->cont();
  };
  this->_fd.async_read_some(asio::buffer(b), recv_fn);
  yield();
  if (error)
    throw std::runtime_error{error.message()};
  return size;
}

future<size_t>
descriptor::async_read(asio::mutable_buffers_1 b)
{
  auto task = [&, b] {
    return this->read(b);
  };
  return future<size_t>{task};
}

future<size_t>
descriptor::async_write(asio::const_buffer b)
{
  auto task = [&, b] {
    return this->write(b);
  };
  return future<size_t>{task};
}

} /* tnet */
