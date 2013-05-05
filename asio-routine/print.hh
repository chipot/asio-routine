#pragma once
#ifndef PRINT_K0KKBLVK
#define PRINT_K0KKBLVK

// Yes, this is a little heavy on compile time.
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <iterator>
#include <algorithm>
#include <sstream>

template <class K, class V>
std::ostream&
operator<<(std::ostream& out, std::pair<K, V> const &p);

namespace detail {

template <typename IT>
std::string join(IT it, IT ite, std::string const& sep = ",")
{
    int n = 0;
    std::stringstream res;
    for (; it != ite; ++it)
    {
        auto const& val = *it;
        if (n++)
            res << sep;
        res << val;
    }
    return res.str();
}
    
} /* detail */

template <class K, class V>
std::ostream&
operator<<(std::ostream& out, std::pair<K, V> const &p)
{
    return out << p.first <<  ": " << p.second;
}

template <class K, class V, class ...OTHER>
std::ostream&
operator<<(std::ostream& out, std::multimap<K, V, OTHER...> const &m)
{
    int n = 0;
    out << "+{";
    for(auto it = m.begin(), ite = m.end();
       it != ite;
       it = m.upper_bound(it->first))
    {
      auto itpair = m.equal_range(it->first);
      out << (n++ ? ", " : "") << it->first << ": [";
      int m = 0;
      for (;itpair.first != itpair.second; ++itpair.first)
      {
        auto const& item = *itpair.first;
        out << (m++ ? ", " : "") << item.second;
      }
      out << "]";
    }
    out << "}+";
    return out;
}

template <class K, class V, class ...OTHER>
std::ostream&
operator<<(std::ostream& out, std::map<K, V, OTHER...> const &m)
{
    return out << "{" << detail::join(begin(m), end(m), ", ") << "}";
}

template <typename T, typename ...OTHER>
std::ostream&
operator<<(std::ostream& out, std::vector<T, OTHER...> const &v)
{
    return out << "[" << detail::join(begin(v), end(v), ", ") << "]";
}

template <typename T, typename ...OTHER>
std::ostream&
operator<<(std::ostream& out, std::list<T, OTHER...> const &l)
{
    return out << "(" << detail::join(begin(l), end(l), ", ") << ")";
}

template <class T>
std::ostream&
operator<<(std::ostream& out, std::initializer_list<T> const &s)
{
    return out << "{" << detail::join(begin(s), end(s), ", ") << "}";
}

template <class HEAD>
void _print(std::ostream& out, HEAD &&head)
{
    out << std::forward<HEAD>(head);
}

template <class HEAD, class ...TAIL>
void _print(std::ostream& out, HEAD &&head, TAIL && ...tail)
{
    out << std::forward<HEAD>(head) << " ";
    _print(out, std::forward<TAIL>(tail)...);
}

template <class ...ARGS>
void print(std::ostream& out, ARGS && ...args)
{
    _print(out, std::forward<ARGS>(args)...);
    out << std::endl;
}

template <class ...ARGS>
void print(ARGS && ...args)
{
    _print(std::cout, std::forward<ARGS>(args)...);
    std::cout << std::endl;
}

template <class ...ARGS>
std::string sprint(ARGS && ...args)
{
    std::stringstream ss;
    _print(ss, std::forward<ARGS>(args)...);
    return ss.str();
}

template <class IT>
void
_printfmt(std::ostream& out, IT it_begin, IT it_end)
{
  std::copy(it_begin, it_end,
            std::ostream_iterator<typename IT::value_type>(out, ""));
}

template <class IT, class HEAD, class ...TAIL>
void
_printfmt(std::ostream& out, IT it_begin, IT it_end, HEAD&& head, TAIL&&... tail)
{
  static std::string sep = "%";

  auto it = std::find_first_of(it_begin, it_end, begin(sep), end(sep));
  std::copy(it_begin, it,
            std::ostream_iterator<typename IT::value_type>(out, ""));
  if (it != it_end)
  {
    out << head;
    _printfmt(out, it + 1, it_end, std::forward<TAIL>(tail)...);
  }
}

template <class ...ARGS>
void
printfmt(std::string const& fmt, ARGS &&... args)
{
  _printfmt(std::cout, begin(fmt), end(fmt), std::forward<ARGS>(args)...);
}

template <class ...ARGS>
std::string
sprintfmt(std::string const& fmt, ARGS &&... args)
{
  std::stringstream ss;
  _printfmt(ss, begin(fmt), end(fmt), std::forward<ARGS>(args)...);
  return ss.str();
}

#endif /* end of include guard: PRINT_K0KKBLVK */
