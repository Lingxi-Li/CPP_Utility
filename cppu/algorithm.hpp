// Copyright (c) 2016, Lingxi Li <lilingxi.cs@gmail.com>
// All rights reserved.
// CPP Utility Library (https://github.com/Lingxi-Li/CPP_Utility)

#ifndef CPPU_ALGORITHM_HPP_
#define CPPU_ALGORITHM_HPP_

#include <algorithm>
#include <array>
#include <utility>

namespace cppu {

namespace detail {

// performs `swap(x, y)`, looking up in both namespace `std` and that of `T`
// (by ADL)
template <typename T>
void iswap(T& x, T& y, int) {
  // fallbacks to `std::swap()` if ADL failed
  using std::swap;
  swap(x, y);
}

// performs `x.swap(y)`; disabled if `x.swap(y)` is ill-formed
template <typename T>
auto iswap(T& x, T& y, char) -> decltype(x.swap(y)) {
  return x.swap(y);
}

} // namespace detail

/// Introspective swap. Performs `x.swap(y)` if possible. Otherwise, performs
/// `swap(x, y)`, looking up in both namespace `std` and that of `T` (by ADL).
template <typename T>
void iswap(T& x, T& y) {
  // the last argument makes `detail::iswap(int&, int&, char)`, if enabled,
  // a better match than `detail::iswap(int&, int&, int)`, implementing a
  // preference of `x.swap(y)` over `swap(x, y)`; uses `static_cast<void>()`
  // to ignore possible (though very unlikely) return value from
  // `detail::iswap(int&, int&, char)`
  static_cast<void>(detail::iswap(x, y, ' '));
}

/// Introspective swap for arrays. For each pair of elements `x[i]` and `y[i]`,
/// performs `x[i].swap(y[i])` if possible. Otherwise, performs
/// `swap(x[i], y[i])`, looking up in both namespace `std` and that of `T`
/// (by ADL). Introspective swap is invoked recursively when necessary.
template <typename T, std::size_t n>
void iswap(T (&x)[n], T (&y)[n]) {
  for (std::size_t i = 0; i < n; ++i) {
    iswap(x[i], y[i]);
  }
}

/// Introspective swap for `std::array`. For each pair of elements `x[i]` and
/// `y[i]`, performs `x[i].swap(y[i])` if possible. Otherwise, performs
/// `swap(x[i], y[i])`, looking up in both namespace `std` and that of `T`
/// (by ADL). Introspective swap is invoked recursively when necessary.
template <typename T, std::size_t n>
void iswap(std::array<T, n>& x, std::array<T, n>& y) {
  for (std::size_t i = 0; i < n; ++i) {
    iswap(x[i], y[i]);
  }
}

} // namespace cppu

#endif // CPPU_ALGORITHM_HPP_
