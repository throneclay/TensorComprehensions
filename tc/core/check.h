#pragma once

#include <cstdint>
#include <iosfwd>
#include <sstream>
#include <string>

#include "tc/core/utils/type_traits.h"

#define TC_CHECK(condition) \
  tc::detail::tc_check(static_cast<bool>(condition), __FILE__, __LINE__)
#define TC_CHECK_EQ(x, y) tc::detail::tc_check_eq(x, y, __FILE__, __LINE__)
#define TC_CHECK_NE(x, y) tc::detail::tc_check_ne(x, y, __FILE__, __LINE__)
#define TC_CHECK_LT(x, y) tc::detail::tc_check_lt(x, y, __FILE__, __LINE__)
#define TC_CHECK_GT(x, y) tc::detail::tc_check_gt(x, y, __FILE__, __LINE__)
#define TC_CHECK_GE(x, y) tc::detail::tc_check_ge(x, y, __FILE__, __LINE__)
#define TC_CHECK_LE(x, y) tc::detail::tc_check_le(x, y, __FILE__, __LINE__)

namespace tc {

namespace detail {
class Checker {
 public:
  Checker(bool condition, std::string location, std::string baseErrorMsg);
  ~Checker() noexcept(false);

  template <typename T>
  typename std::enable_if<!tc::is_std_container<T>::value, Checker&>::type
  operator<<(const T& msg) {
    std::stringstream ss;
    ss << additionalMsg_ << msg;
    additionalMsg_ = ss.str();
    return *this;
  }

  template <typename C>
  typename std::enable_if<tc::is_std_container<C>::value, Checker&>::type
  operator<<(const C& msg) {
    std::stringstream ss;
    ss << additionalMsg_;
    for (const auto& x : msg) {
      ss << x << ',';
    }
    additionalMsg_ = ss.str();
    if (msg.begin() != msg.end()) {
      additionalMsg_.pop_back();
    }
    return *this;
  }

 private:
  bool condition_;
  std::string location_;
  std::string baseErrorMsg_;
  std::string additionalMsg_;
}; // namespace detail

std::string makeLocation(const char* filename, uint64_t lineno);

Checker tc_check(bool condition, const char* filename, uint64_t lineno);

template <typename X, typename Y>
Checker
tc_check_eq(const X& x, const Y& y, const char* filename, uint64_t lineno) {
  std::stringstream ss;
  ss << x << " not equal to " << y;
  return Checker(x == y, makeLocation(filename, lineno), ss.str());
}

template <typename X, typename Y>
Checker
tc_check_ne(const X& x, const Y& y, const char* filename, uint64_t lineno) {
  std::stringstream ss;
  ss << x << " equal to " << y;
  return Checker(x != y, makeLocation(filename, lineno), ss.str());
}

template <typename X, typename Y>
Checker
tc_check_lt(const X& x, const Y& y, const char* filename, uint64_t lineno) {
  std::stringstream ss;
  ss << x << " not less than " << y;
  return Checker(x < y, makeLocation(filename, lineno), ss.str());
}

template <typename X, typename Y>
Checker
tc_check_gt(const X& x, const Y& y, const char* filename, uint64_t lineno) {
  std::stringstream ss;
  ss << x << " not greater than " << y;
  return Checker(x > y, makeLocation(filename, lineno), ss.str());
}

template <typename X, typename Y>
Checker
tc_check_le(const X& x, const Y& y, const char* filename, uint64_t lineno) {
  std::stringstream ss;
  ss << x << " not less than or equal to " << y;
  return Checker(x <= y, makeLocation(filename, lineno), ss.str());
}

template <typename X, typename Y>
Checker
tc_check_ge(const X& x, const Y& y, const char* filename, uint64_t lineno) {
  std::stringstream ss;
  ss << x << " not greater than or equal to " << y;
  return Checker(x >= y, makeLocation(filename, lineno), ss.str());
}

} // namespace detail
} // namespace tc
