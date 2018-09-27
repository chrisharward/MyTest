#pragma once

namespace STL {
struct nullopt_t {
  explicit constexpr nullopt_t(int) {}
};

struct in_place_t {
  explicit in_place_t() = default;
};
inline constexpr in_place_t in_place{};

struct bad_optional_access {};

template <typename T> class optional {
public:
  using value_type = T;

  template<typename U> friend class optional;

  constexpr optional() noexcept = default;
  constexpr optional(STL::nullopt_t) noexcept {}

  constexpr optional(const optional &other) 
     : contains_val_m{ other.contains_val_m }, val_m{ other.val_m } {}
  constexpr optional(optional &&other) noexcept 
     : contains_val_m{ other.contains_val_m }, val_m{ std::move(other.val_m) } {}

  template <class... Args, typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
  constexpr explicit optional(std::in_place_t, Args &&... args)
      : contains_val_m{true}, val_m{std::forward<Args>(args)...} {}

  template <class U, typename = std::enable_if_t<std::is_constructible_v<T, const U &>>>
  explicit optional(const optional<U> &other) {
    contains_val_m = other.contains_val_m;
    val_m = other.val_m;
  }

  template <class U, typename = std::enable_if<std::is_constructible_v<T, U &&>>>
  explicit optional(optional<U> &&other)
      : contains_val_m{other.contains_val_m}, val_m{std::move(other.val_m)} {}

  template <class U = value_type,
            typename = std::enable_if_t<std::is_constructible_v<T, U &&>>>
  constexpr explicit optional(U &&value)
      : contains_val_m{true}, val_m{std::move(value)} {}

  optional& operator=(STL::nullopt_t) noexcept {
     contains_val_m = false;
     val_m.T::~T();
     return *this;
  }
  optional& operator=(const optional& other) = default;
  optional& operator=(optional&& other) noexcept = default;
  template<class U = T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<U>, optional<T>> &&
     std::is_constructible_v<T, U> &&
     std::is_assignable_v<T&, U>>>
  optional& operator=(U&& value) {
     contains_val_m = true;
     val_m = std::move(value);
     return *this;
  }
  template<class U>
  optional& operator=(const optional<U>& other) {
     contains_val_m = other.contains_val_m;
     val_m = other.val_m;
     return *this;
  }
  template<class U>
  optional& operator=(optional<U>&& other) {
     contains_val_m = other.contains_val_m;
     val_m = std::move(other.val_m);
     return *this;
  }

  constexpr const T* operator->() const {
     return &val_m;
  }
  constexpr T* operator->() {
     return &val_m;
  }
  constexpr const T& operator*() const& {
     return val_m;
  }
  constexpr T& operator*() & {
     return val_m;
  }
  constexpr const T&& operator*() const&& {
     return val_m;
  }
  constexpr T&& operator*() && {
     return val_m;
  }

  constexpr explicit operator bool() const noexcept { return has_value(); }
  constexpr bool has_value() const noexcept { return contains_val_m; }

  constexpr T &value() & {
    if (!contains_val_m)
      throw bad_optional_access{};
    return val_m;
  }
  constexpr const T &value() const & {
    if (!contains_val_m)
      throw bad_optional_access{};
    return val_m;
  }

private:
  bool contains_val_m = false;
  T val_m;
};
} // namespace STL
