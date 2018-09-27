#pragma once

#include "algorithm.h"
#include <iterator>

namespace STL {

template <class T, std::size_t N> class array {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  template <typename POINTER, typename REFERENCE> struct array_iterator_base {
    using value_type = typename array::value_type;
    using difference_type = typename array::difference_type;
    using reference = typename REFERENCE;
    using pointer = POINTER;
    using iterator_category = std::random_access_iterator_tag;

    constexpr reference operator*() const noexcept { return *_ptr; }
    constexpr pointer operator->() const noexcept { return _ptr; }
    constexpr reference operator[](difference_type n) noexcept {
      return _ptr[n];
    }

    constexpr array_iterator_base<POINTER, REFERENCE> &operator++() noexcept {
      ++_ptr;
      return *this;
    }
    constexpr array_iterator_base<POINTER, REFERENCE> operator++(int)noexcept {
      auto _tmp = _ptr;
      ++_ptr;
      return {_tmp};
    }

    constexpr array_iterator_base<POINTER, REFERENCE> &operator--() noexcept {
      --_ptr;
      return *this;
    }
    constexpr array_iterator_base<POINTER, REFERENCE> operator--(int)noexcept {
      auto _tmp = _ptr;
      --_ptr;
      return {_tmp};
    }

    constexpr void operator+=(difference_type n) noexcept { _ptr += n; }
    constexpr array_iterator_base<POINTER, REFERENCE>
    operator+(difference_type n) noexcept {
      return {_ptr + n};
    }
    constexpr void operator-=(difference_type n) noexcept { _ptr -= n; }
    constexpr array_iterator_base<POINTER, REFERENCE>
    operator-(difference_type n) noexcept {
      return {_ptr - n};
    }

    constexpr difference_type
    operator-(const array_iterator_base<POINTER, REFERENCE> &other) noexcept {
      return _ptr - other._ptr;
    }

    constexpr bool
    operator==(const array_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr == other._ptr;
    }
    constexpr bool
    operator!=(const array_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr != other._ptr;
    }

    constexpr bool
    operator<(const array_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr < other._ptr;
    }
    constexpr bool
    operator<=(const array_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr <= other._ptr;
    }
    constexpr bool
    operator>(const array_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr > other._ptr;
    }
    constexpr bool
    operator>=(const array_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr >= other._ptr;
    }

    pointer _ptr = nullptr;
  };

  template <typename POINTER, typename REFERENCE>
  struct array_reverse_iterator_base {
    using value_type = typename array::value_type;
    using difference_type = typename array::difference_type;
    using reference = typename REFERENCE;
    using pointer = POINTER;
    using iterator_category = std::random_access_iterator_tag;

    constexpr reference operator*() const noexcept { return *(_ptr - 1); }
    constexpr pointer operator->() const noexcept { return _ptr - 1; }
    constexpr reference operator[](difference_type n) noexcept {
      return _ptr[-n - 1];
    }

    constexpr array_reverse_iterator_base<POINTER, REFERENCE> &
    operator++() noexcept {
      --_ptr;
      return *this;
    }
    constexpr array_reverse_iterator_base<POINTER, REFERENCE>
    operator++(int)noexcept {
      auto _tmp = _ptr;
      --_ptr;
      return {_tmp};
    }

    constexpr array_reverse_iterator_base<POINTER, REFERENCE> &
    operator--() noexcept {
      ++_ptr;
      return *this;
    }
    constexpr array_reverse_iterator_base<POINTER, REFERENCE>
    operator--(int)noexcept {
      auto _tmp = _ptr;
      ++_ptr;
      return {_tmp};
    }

    constexpr void operator+=(difference_type n) noexcept { _ptr -= n; }
    constexpr array_reverse_iterator_base<POINTER, REFERENCE>
    operator+(difference_type n) noexcept {
      return {_ptr - n};
    }
    constexpr void operator-=(difference_type n) noexcept { _ptr += n; }
    constexpr array_reverse_iterator_base<POINTER, REFERENCE>
    operator-(difference_type n) noexcept {
      return {_ptr + n};
    }

    constexpr difference_type operator-(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) noexcept {
      return other._ptr - _ptr;
    }

    constexpr bool operator==(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr == other._ptr;
    }
    constexpr bool operator!=(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return _ptr != other._ptr;
    }

    constexpr bool operator<(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return other._ptr < _ptr;
    }
    constexpr bool operator<=(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return other._ptr <= _ptr;
    }
    constexpr bool operator>(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return other._ptr > _ptr;
    }
    constexpr bool operator>=(
        const array_reverse_iterator_base<POINTER, REFERENCE> &other) const
        noexcept {
      return other._ptr >= _ptr;
    }

    pointer _ptr = nullptr;
  };

  using const_array_iterator =
      array_iterator_base<typename array::const_pointer,
                          typename array::const_reference>;
  using array_iterator =
      array_iterator_base<typename array::pointer, typename array::reference>;

  using const_array_reverse_iterator =
      array_reverse_iterator_base<typename array::const_pointer,
                                  typename array::const_reference>;

  using array_reverse_iterator =
      array_reverse_iterator_base<typename array::pointer,
                                  typename array::reference>;

  using iterator = array_iterator;
  using const_iterator = const_array_iterator;
  using reverse_iterator = array_reverse_iterator;
  using const_reverse_iterator = const_array_reverse_iterator;

  constexpr reference operator[](size_type n) noexcept { return _data[n]; }

  constexpr const_reference operator[](size_type n) const noexcept {
    return _data[n];
  }

  constexpr reference at(size_type n) {
    if (n < N) {
      return _data[n];
    } else {
      throw std::exception("Index out of range.");
    }
  }

  constexpr const_reference at(size_type n) const {
    if (n < N) {
      return _data[n];
    } else {
      throw std::exception("Index out of range.");
    }
  }

  constexpr reference front() noexcept { return _data[0]; }

  constexpr const_reference front() const noexcept { return _data[0]; }

  constexpr reference back() noexcept { return _data[N - 1]; }

  constexpr const_reference back() const noexcept { return _data[N - 1]; }

  constexpr pointer data() noexcept { return _data; }

  constexpr const_pointer data() const noexcept { return _data; }

  constexpr bool empty() const noexcept { return false; }

  constexpr size_type size() const noexcept { return N; }

  constexpr size_type max_size() const noexcept { return size(); }

  void fill(const T &value) {
    for (auto &val : _data) {
      val = value;
    }
  }

  void swap(array &other) noexcept {
    for (auto i = 0u; i < N; ++i) {
      STL::swap(_data[i], other[i]);
    }
  }

  constexpr iterator begin() noexcept { return {&_data[0]}; }
  constexpr const_iterator begin() const noexcept { return {&_data[0]}; }
  constexpr const_iterator cbegin() const noexcept { return {&_data[0]}; }

  constexpr iterator end() noexcept { return {&_data[N]}; }
  constexpr const_iterator end() const noexcept { return {&_data[N]}; }
  constexpr const_iterator cend() const noexcept { return {&_data[N]}; }

  constexpr reverse_iterator rbegin() noexcept { return {&_data[N]}; }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return {&_data[N]};
  }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return {&_data[N]};
  }

  constexpr reverse_iterator rend() noexcept { return {&_data[0]}; }
  constexpr const_reverse_iterator rend() const noexcept { return {&_data[0]}; }
  constexpr const_reverse_iterator crend() const noexcept {
    return {&_data[0]};
  }

  T _data[N];
};

template <class T, std::size_t N>
constexpr bool operator==(const array<T, N> &a1,
                          const array<T, N> &a2) noexcept {
  return STL::equal(std::begin(a1), std::end(a1), std::begin(a2), std::end(a2));
}

template <class T, std::size_t N>
constexpr bool operator!=(const array<T, N> &a1,
                          const array<T, N> &a2) noexcept {
  return !STL::equal(std::begin(a1), std::end(a1), std::begin(a2),
                     std::end(a2));
}

template <class T, std::size_t N>
bool operator<(const array<T, N> &lhs, const array<T, N> &rhs) {
  return STL::lexicographical_compare(std::begin(lhs), std::end(lhs),
                                      std::begin(rhs), std::end(rhs));
}

template <size_t I, class T, size_t N>
constexpr T &get(array<T, N> &a) noexcept {
  return a[I];
}

template <size_t I, class T, size_t N>
constexpr T &&get(array<T, N> &&a) noexcept {
  return std::move(a[I]);
}

template <size_t I, class T, size_t N>
constexpr const T &get(const array<T, N> &a) noexcept {
  return a[I];
}

template <size_t I, class T, size_t N>
constexpr const T &&get(const array<T, N> &&a) noexcept {
  return std::move(a[I]);
}

template <class T, std::size_t N>
void swap(array<T, N> &lhs, array<T, N> &rhs) noexcept {
  lhs.swap(rhs);
}

}; // namespace STL