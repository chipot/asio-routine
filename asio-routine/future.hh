#pragma once
#ifndef FUTURE_IFXAGCIE
#define FUTURE_IFXAGCIE

#include <future>
#include <algorithm>
#include <utility>
#include <cassert>

namespace tnet {

template <class T>
class future
{
 private:
  std::packaged_task<T()> _task;
  bool _used = false;
 public:
  operator T()
  {
      return this->get();
  }

  T get()
  {
    assert(this->_used == false);
    this->_used = true;
    std::future<T> f = _task.get_future();
    _task();
    return f.get();
  }

  future(std::function<T()> f)
      : _task(f)
  {
  }

  future(future&& f)
    : _task{}
    , _used{false}
  {
    std::swap(this->_task, f._task);
    std::swap(this->_used, f._used);
  }
  ~future()
  {
    assert(this->_used);
  }
};

template <typename T, typename ...ARGS>
std::function<future<T> (ARGS...)>wrap(std::function<T (ARGS...)> const &f)
{
  return [&] (ARGS && ...args) -> future<T>
  {
    return future<T>{[&] {return f(std::forward<ARGS>(args)...);}};
  };
}

} /* tnet */

#endif /* end of include guard: FUTURE_IFXAGCIE */
