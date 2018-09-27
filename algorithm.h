#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace STL {
template <typename IT, typename UNARY_PRED>
bool all_of(IT first, IT last, UNARY_PRED &&pred) noexcept {
  while (first != last) {
    if (!pred(*first++)) {
      return false;
    }
  }
  return true;
}

template <typename IT, typename UNARY_PRED>
bool any_of(IT first, IT last, UNARY_PRED &&pred) noexcept {
  bool bAnyOf = false;
  while (first != last) {
    bAnyOf |= pred(*first++);
  }
  return bAnyOf;
}

template <typename IT, typename UNARY_PRED>
bool none_of(IT first, IT last, UNARY_PRED &&pred) noexcept {
  bool bNoneOf = true;
  while (first != last) {
    bNoneOf &= !pred(*first++);
  }
  return bNoneOf;
}

template <typename IT, typename UNARY_FUNC>
UNARY_FUNC for_each(IT first, IT last, UNARY_FUNC func) noexcept {
  while (first != last) {
    func(*first++);
  }
  return std::move(func);
}

template <typename IT, typename SIZE, typename UNARY_FUNC>
void for_each_n(IT first, SIZE n, UNARY_FUNC func) noexcept {
  for (auto i = SIZE{}; i < n; ++i) {
    func(*first++);
  }
}

template <typename IT, typename T>
typename std::iterator_traits<IT>::difference_type count(IT first, IT end,
                                                         T val) noexcept {
  auto res = typename std::iterator_traits<IT>::difference_type{};
  while (first != end) {
    if (*first++ == val) {
      ++res;
    }
  }
  return res;
}

template <typename IT, typename UNARY_PRED>
typename std::iterator_traits<IT>::difference_type
count_if(IT first, IT end, UNARY_PRED pred) noexcept {
  auto res = typename std::iterator_traits<IT>::difference_type{};
  while (first != end) {
    if (pred(*first++)) {
      res++;
    }
  }
  return res;
}

template <typename IT1, typename IT2>
std::pair<IT1, IT2> mismatch(IT1 begin1, IT1 end1, IT2 begin2) noexcept {
  for (; begin1 != end1; ++begin1, ++begin2) {
    if (*begin1 != *begin2) {
      return {begin1, begin2};
    }
  }
  return {begin1, begin2};
}

template <typename IT1, typename IT2>
std::pair<IT1, IT2> mismatch(IT1 begin1, IT1 end1, IT2 begin2,
                             IT2 end2) noexcept {
  for (; begin1 != end1 && begin2 != end2; ++begin1, ++begin2) {
    if (*begin1 != *begin2) {
      return {begin1, begin2};
    }
  }
  return {begin1, begin2};
}

template <typename IT1, typename IT2, typename BINARY_PRED>
std::pair<IT1, IT2> mismatch(IT1 begin1, IT1 end1, IT2 begin2, IT2 end2,
                             BINARY_PRED pred) noexcept {
  for (; begin1 != end1 && begin2 != end2; ++begin1, ++begin2) {
    if (!pred(*begin1, *begin2)) {
      return {begin1, begin2};
    }
  }
  return {begin1, begin2};
}

template <typename IT1, typename IT2, typename BINARY_PRED>
bool equal(IT1 begin1, IT1 end1, IT2 begin2, IT2 end2,
           BINARY_PRED pred) noexcept {
  if (std::distance(begin1, end1) != std::distance(begin2, end2))
    return false;

  for (; begin1 != end1 && begin2 != end2; ++begin1, ++begin2) {
    if (!pred(*begin1, *begin2))
      return false;
  }
  return true;
}

template <typename IT1, typename IT2>
bool equal(IT1 begin1, IT1 end1, IT2 begin2, IT2 end2) noexcept {
  if (std::distance(begin1, end1) != std::distance(begin2, end2))
    return false;

  for (; begin1 != end1 && begin2 != end2; ++begin1, ++begin2) {
    if (*begin1 != *begin2)
      return false;
  }
  return true;
}

template <typename IT1, typename IT2>
bool equal(IT1 begin1, IT1 end1, IT2 begin2) noexcept {
  for (; begin1 != end1; ++begin1, ++begin2) {
    if (*begin1 != *begin2)
      return false;
  }
  return true;
}

template <typename IT, typename T>
IT find(IT begin, IT end, const T &val) noexcept {
  for (; begin != end; ++begin) {
    if (*begin == val) {
      return begin;
    }
  }
  return end;
}

template <typename IT, typename UNARY_PRED>
IT find_if(IT begin, IT end, UNARY_PRED &&pred) noexcept {
  for (; begin != end; ++begin) {
    if (pred(*begin)) {
      return begin;
    }
  }
  return end;
}

template <typename IT, typename UNARY_PRED>
IT find_if_not(IT begin, IT end, UNARY_PRED &&pred) {
  return STL::find_if(begin, end,
                      [&pred](const auto &val) { return !pred(val); });
}

template <typename IT1, typename IT2>
IT1 find_end(IT1 begin, IT1 end, IT2 begin_s, IT2 end_s) noexcept {
  return find_end(
      begin, end, begin_s, end_s,
      [](const auto &val1, const auto &val2) { return val1 == val2; });
}

template <typename IT1, typename IT2, typename BINARY_PRED>
IT1 find_end(IT1 begin, IT1 end, IT2 begin_s, IT2 end_s,
             BINARY_PRED &&pred) noexcept {
  if (begin_s == end_s)
    return end;

  IT1 found = end;
  while (begin != end) {
    IT1 curr = begin;
    IT2 curr_s = begin_s;
    while (curr != end) {
      if (!pred(*curr, *curr_s)) {
        break;
      }
      ++curr, ++curr_s;
      if (curr_s == end_s) {
        found = begin;
        break;
      }
    }
    ++begin;
  }

  return found;
}

template <typename IT1, typename IT2>
IT1 find_first_of(IT1 begin, IT1 end, IT2 begin_s, IT2 end_s) noexcept {
  return find_first_of(
      begin, end, begin_s, end_s,
      [](const auto &val1, const auto &val2) { return val1 == val2; });
}

template <typename IT1, typename IT2, typename BINARY_PRED>
IT1 find_first_of(IT1 begin, IT1 end, IT2 begin_s, IT2 end_s,
                  BINARY_PRED &&pred) noexcept {
  if (begin_s == end_s)
    return end;

  while (begin != end) {
    IT1 curr = begin;
    IT2 curr_s = begin_s;
    while (curr != end) {
      if (!pred(*curr, *curr_s)) {
        break;
      }
      ++curr_s, ++curr;

      if (curr_s == end_s) {
        return begin;
      }
    }
    ++begin;
  }
  return end;
}

template <typename IT1, typename IT2>
IT1 search(IT1 begin, IT1 end, IT2 begin_s, IT2 end_s) noexcept {
  return find_first_of(begin, end, begin_s, end_s);
}

template <typename IT1, typename IT2, typename BINARY_PRED>
IT1 search(IT1 begin, IT1 end, IT2 begin_s, IT2 end_s,
           BINARY_PRED &&pred) noexcept {
  return find_first_of(begin, end, begin_s, end_s,
                       std::forward<BINARY_PRED>(pred));
}

template <typename IT, typename T, typename SIZE>
IT search_n(IT begin, IT end, SIZE count, const T &val) noexcept {
  if (count <= 0)
    return end;

  auto count_found = SIZE{};
  IT begin_match = end;
  for (; begin != end; ++begin) {
    if (*begin == val) {
      if (count_found++ == 0) {
        begin_match = begin;
      }
    } else {
      count_found = 0;
    }

    if (count_found == count) {
      return begin_match;
    }
  }

  return end;
}

template <typename IT> IT adjacent_find(IT begin, IT end) {
  if (begin == end)
    return end;

  for (IT next = begin + 1; next != end; ++begin, ++next) {
    if (*begin == *next) {
      return begin;
    }
  }
  return end;
}

template <typename FORWARD_IT, typename OUTPUT_IT>
OUTPUT_IT copy(FORWARD_IT begin, FORWARD_IT end, OUTPUT_IT begin_d) noexcept {
  while (begin != end) {
    *begin_d++ = *begin++;
  }
  return begin_d;
}

template <typename FORWARD_IT, typename OUTPUT_IT, typename UNARY_PRED>
OUTPUT_IT copy_if(FORWARD_IT begin, FORWARD_IT end, OUTPUT_IT begin_d,
                  UNARY_PRED &&pred) noexcept {
  while (begin != end) {
    if (pred(*begin)) {
      *begin_d++ = *begin;
    }
    ++begin;
  }
  return begin_d;
}

template <typename INPUTIT, typename SIZE, typename OUTPUTIT>
OUTPUTIT copy_n(INPUTIT src, SIZE count, OUTPUTIT dest) noexcept {
  for (auto ind = SIZE{}; ind < count; ++ind) {
    *dest++ = *src++;
  }
  return dest;
}

template <typename BIDIT1, typename BIDIT2>
BIDIT2 copy_backwards(BIDIT1 begin, BIDIT1 end, BIDIT2 end_d) noexcept {
  while (begin != end) {
    *--end_d = *--end;
  }
  return end_d;
}

template <typename INPUTIT, typename OUTPUTIT>
OUTPUTIT move(INPUTIT begin, INPUTIT end, OUTPUTIT begin_d) noexcept {
  while (begin != end) {
    *begin_d++ = std::move(*begin++);
  }
  return begin_d;
}

template <typename BIDIRIT1, typename BIDIRIT2>
BIDIRIT2 move_backwards(BIDIRIT1 begin, BIDIRIT1 end, BIDIRIT2 end_d) noexcept {
  while (begin != end) {
    *--end_d = std::move(*--end);
  }
  return end_d;
}

template <typename ForwardIT, typename T>
void fill(ForwardIT begin, ForwardIT end, const T &val) noexcept {
  while (begin != end) {
    *begin++ = val;
  }
}

template <typename ForwardIT, typename SIZE, typename T>
void fill_n(ForwardIT begin, ForwardIT end, SIZE count, const T &val) noexcept {
  for (auto num = SIZE{}; begin != end && num < count; ++num) {
    *begin++ = val;
  }
}

template <typename InputIt, typename OutputIt, typename UnaryOp>
OutputIt transform(InputIt begin, InputIt end, OutputIt begin_d,
                   UnaryOp op) noexcept {
  while (begin != end) {
    *begin_d++ = op(*begin++);
  }
  return begin_d;
}

template <typename InputIt1, typename InputIt2, typename OutputIt,
          typename BinaryOp>
OutputIt transform(InputIt1 begin1, InputIt1 end1, InputIt2 begin2,
                   OutputIt begin_d, BinaryOp op) noexcept {
  while (begin1 != end1) {
    *begin_d++ = op(*begin1++, *begin2++);
  }
  return begin_d;
}

template <class ForwardIt, class Generator>
void generate(ForwardIt first, ForwardIt last, Generator g) {
  while (first != last) {
    *first++ = g();
  }
}

template <class OutputIt, class Size, class Generator>
OutputIt generate_n(OutputIt first, Size count, Generator g) {
  for (auto generated = Size{}; generated < count; ++generated) {
    *first++ = g();
  }
  return first;
}

template <class ForwardIt, class T>
ForwardIt remove(ForwardIt first, ForwardIt last, const T &value) {
  first = STL::find(first, last, value);
  if (first != last) {
    for (auto i = first + 1; i != last; ++i) {
      if (*i != value) {
        *first = std::move(*i);
        first = i;
      }
    }
  }
  return first;
}

template <class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p) {
  first = STL::find(first, last, p);
  if (first != last) {
    for (auto i = first + 1; i != last; ++i) {
      if (!p(*i)) {
        *first = std::move(*i);
        first = i;
      }
    }
  }
  return first;
}

template <class InputIt, class OutputIt, class T>
OutputIt remove_copy(InputIt first, InputIt last, OutputIt d_first,
                     const T &value) {
  while (first != last) {
    if (*first != value) {
      *d_first++ = *first;
    }
    first++;
  }
  return d_first;
}

template <class InputIt, class OutputIt, class UnaryPredicate>
OutputIt remove_copy_if(InputIt first, InputIt last, OutputIt d_first,
                        UnaryPredicate p) {
  while (first != last) {
    if (p(*first)) {
      *d_first++ = *first;
    }
    first++;
  }
  return d_first;
}

template <class ForwardIt, class T>
void replace(ForwardIt first, ForwardIt last, const T &old_value,
             const T &new_value) {
  while (first != last) {
    if (*first == old_value) {
      *first = new_value;
    }
    ++first;
  }
}

template <class ForwardIt, class UnaryPredicate, class T>
void replace_if(ForwardIt first, ForwardIt last, UnaryPredicate p,
                const T &new_value) {
  while (first != last) {
    if (p(*first)) {
      *first = new_value;
    }
    ++first;
  }
}

template <class InputIt, class OutputIt, class T>
OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first,
                      const T &old_value, const T &new_value) {
  while (first != last) {
    if (*d_first == old_value) {
      *first++ = new_value;
    } else {
      *first++ = *d_first;
    }
    ++d_first;
  }
  return d_first;
}

template <class InputIt, class OutputIt, class UnaryPredicate, class T>
OutputIt replace_copy_if(InputIt first, InputIt last, OutputIt d_first,
                         UnaryPredicate p, const T &new_value) {
  while (first != last) {
    if (p(*d_first)) {
      *first++ = new_value;
    } else {
      *first++ = *d_first;
    }
    ++d_first;
  }
  return d_first;
}

template <class T>
typename std::enable_if<
    std::is_move_constructible_v<T> && std::is_move_assignable_v<T>, void>::type
swap(T &a, T &b) noexcept(std::is_nothrow_move_assignable_v<T>
                              &&std::is_nothrow_move_constructible_v<T>) {
  T tmp = std::move(b);
  b = std::move(a);
  a = std::move(tmp);
}

template <class T>
typename std::enable_if<!(std::is_move_constructible_v<T> &&
                          std::is_move_assignable_v<T>),
                        void>::type
swap(T &a, T &b) noexcept(std::is_nothrow_copy_assignable_v<T>
                              &&std::is_nothrow_copy_constructible_v<T>) {
  T tmp = b;
  b = a;
  a = tmp;
}

template <typename T, size_t N, size_t I> struct swap_impl {
  static void swap(T (&a)[N],
                   T (&b)[N]) noexcept(std::is_nothrow_swappable_v<T>) {
    STL::swap(a[I], b[I]);
    swap_impl<T, N, I - 1>::swap(a, b);
  }
};

template <typename T, size_t N> struct swap_impl<T, N, 0> {
  static void swap(T (&a)[N],
                   T (&b)[N]) noexcept(std::is_nothrow_swappable_v<T>) {
    STL::swap(a[0], b[0]);
  }
};

template <typename T, size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept(std::is_nothrow_swappable_v<T>) {
  swap_impl<T, N, N - 1>::swap(a, b);
}

template <class ForwardIt1, class ForwardIt2>
void iter_swap(ForwardIt1 a, ForwardIt2 b) {
  STL::swap(*a, *b);
}

template <class ForwardIt1, class ForwardIt2>
ForwardIt2 swap_ranges(ForwardIt1 begin, ForwardIt1 end, ForwardIt2 begin_d,
                       ForwardIt2 end_d) {
  while (begin != end && begin_d != end_d) {
    STL::iter_swap(begin++, begin_d++);
  }
  return begin_d;
}

template <typename BidirIt> void reverse(BidirIt first, BidirIt last) {
  while (first != last && first != --last) {
    STL::iter_swap(first++, last);
  }
}

template <typename BidirIt, typename OutputIt>
OutputIt reverse_copy(BidirIt first, BidirIt last, OutputIt d_first) {
  while (first != last) {
    *d_first++ = *--last;
  }
  return d_first;
}

template <class ForwardIt>
ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last) {
  STL::reverse(first, n_first);
  STL::reverse(n_first, last);
  STL::reverse(first, last);

  return first + (last - n_first);
}

template <class ForwardIt, class OutputIt>
OutputIt rotate_copy(ForwardIt first, ForwardIt n_first, ForwardIt last,
                     OutputIt d_first) {
  d_first = STL::copy(n_first, last, d_first);
  return STL::copy(first, n_first, d_first);
}

template <class ForwardIt, class BinaryPredicate>
ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p) {
  ForwardIt next = first;
  while (first != last && ++next != last) {
    if (p(*first++, *next)) {
      last = STL::rotate(next, next + 1, last);
    }
  }
  return last;
}

template <class InputIt, class UnaryPredicate>
bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p) {
  while (first != last) {
    if (!p(*first++))
      break;
  }

  while (first != last) {
    if (p(*first++))
      return false;
  }
  return true;
}

template <typename T>
using is_forward_it =
    std::is_base_of<std::forward_iterator_tag,
                    typename std::iterator_traits<T>::iterator_category>;

template <typename T>
using enable_if_forward_it = std::enable_if_t<is_forward_it<T>::value>;

template <class ForwardIt, class UnaryPredicate,
          typename = enable_if_forward_it<ForwardIt>>
ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p) {
  auto replaceIt = first;
  for (; first != last; ++first) {
    if (p(*first)) {
      STL::iter_swap(first, replaceIt++);
    }
  }
  return replaceIt;
}

template <class BidirIt, class UnaryPredicate>
BidirIt stable_partition(BidirIt first, BidirIt last, UnaryPredicate p) {
  auto replaceIt = STL::find_if_not(first, last, p);
  for (first = replaceIt; first != last; ++first) {
    if (p(*first)) {
      replaceIt = STL::rotate(replaceIt, first, first + 1);
    }
  }
  return replaceIt;
}

template <class BidirIt, class UnaryPredicate>
BidirIt stable_partition_rec(BidirIt first, BidirIt last, UnaryPredicate p) {
  auto dist = std::distance(first, last);
  if (dist == 0) {
    return last;
  } else if (dist == 1) {
    if (p(*first))
      return last;
    else
      return first;
  }
  auto mid = first + dist / 2;
  return STL::rotate(stable_partition_rec(first, mid, p), mid,
                     stable_partition_rec(mid, last, p));
}

template <class ForwardIt, class UnaryPredicate>
ForwardIt partition_point(ForwardIt first, ForwardIt last, UnaryPredicate p) {
  return STL::find_if_not(first, last, p);
}

template <class RandomIt, class Compare>
void heapify(RandomIt first,
             typename std::iterator_traits<RandomIt>::difference_type i,
             typename std::iterator_traits<RandomIt>::difference_type size,
             Compare comp) {
  static_assert(
      std::is_base_of<std::random_access_iterator_tag,
                      std::iterator_traits<RandomIt>::iterator_category>::value,
      "Iterator needs to support random access");

  auto l = 2 * i + 1;
  if (l >= size) {
    return;
  }

  auto max = i;
  if (comp(first[l], first[i])) {
    max = l;
  }

  auto r = l + 1;
  if (r < size) {
    if (comp(first[r], first[max]))
      max = r;
  }

  if (max != i) {
    STL::iter_swap(first + i, first + max);
    heapify(first, max, size, comp);
  }
}

template <class RandomIt, class Compare>
void make_heap(RandomIt first, RandomIt last, Compare comp) {
  static_assert(
      std::is_base_of<std::random_access_iterator_tag,
                      std::iterator_traits<RandomIt>::iterator_category>::value,
      "Iterator needs to support random access");

  auto size = std::distance(first, last);
  for (auto i = size / 2; i >= 0; --i) {
    heapify(first, i, size, comp);
  }
}

template <class RandomIt> void make_heap(RandomIt first, RandomIt last) {
  STL::make_heap(first, last,
                 std::greater<std::iterator_traits<RandomIt>::value_type>());
}

template <class RandomIt, class Compare>
void push_heap(RandomIt first, RandomIt last, Compare comp) {
  static_assert(
      std::is_base_of<std::random_access_iterator_tag,
                      std::iterator_traits<RandomIt>::iterator_category>::value,
      "Iterator needs to support random access");

  auto dist = std::distance(first, last);
  for (auto i = (dist - 1) / 2; i > 0; i /= 2) {
    heapify(first, i, size, comp);
  }
}

template <class RandomIt> void push_heap(RandomIt first, RandomIt last) {
  STL::push_heap(first, last,
                 std::greater<std::iterator_traits<RandomIt>::value_type>());
}

template <class RandomIt, class Compare>
void pop_heap(RandomIt first, RandomIt last, Compare comp) {
  static_assert(
      std::is_base_of<std::random_access_iterator_tag,
                      std::iterator_traits<RandomIt>::iterator_category>::value,
      "Iterator needs to support random access");

  if (first == last)
    return;

  auto newLast = last - 1;
  STL::iter_swap(first, newLast);

  auto i = std::iterator_traits<RandomIt>::difference_type{0};
  auto dist = std::distance(first, newLast);
  heapify(first, i, dist, comp);
}

template <class RandomIt> void pop_heap(RandomIt first, RandomIt last) {
  STL::pop_heap(first, last,
                std::greater<std::iterator_traits<RandomIt>::value_type>());
}

template <class RandomIt, class Compare>
void sort_heap(RandomIt first, RandomIt last, Compare comp) {
  while (first != last) {
    STL::pop_heap(first, last, comp);
    --last;
  }
}

template <class RandomIt> void sort_heap(RandomIt first, RandomIt last) {
  while (first != last) {
    STL::pop_heap(first, --last,
                  std::greater<std::iterator_traits<RandomIt>::value_type>());
  }
}

template <class RandomIt, class Compare>
bool is_heap(RandomIt first, RandomIt last, Compare comp) {
  for (auto i = 0; i != last; ++i) {
    auto l = first + 2 * i + 1;
    if (l == last)
      return true;

    auto curr = first + i;
    if (comp(*l, *curr))
      return false;

    auto r = l + 1;
    if (r == last)
      return true;

    if (comp(*r, *curr))
      return false;
  }
}

template <class RandomIt> bool is_heap(RandomIt first, RandomIt last) {
  return STL::is_heap(
      first, last, std::greater<std::iterator_traits<RandomIt>::value_type>());
}

template <class ForwardIt, class Compare>
bool is_sorted(ForwardIt first, ForwardIt last, Compare comp) {
  if (first == last)
    return true;

  auto next = first + 1;
  while (next != last) {
    if (!comp(*first++, *next++)) {
      return false;
    }
  }
  return true;
}

template <class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare comp) {
  static_assert(
      std::is_base_of<std::random_access_iterator_tag,
                      std::iterator_traits<RandomIt>::iterator_category>::value,
      "Iterator needs to support random access");
  STL::make_heap(first, last, comp);
  STL::sort_heap(first, last, comp);
}

template <class RandomIt> void sort(RandomIt first, RandomIt last) {
  STL::sort(first, last,
            std::greater<std::iterator_traits<RandomIt>::value_type>());
}

template <class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value,
                      Compare comp) {
  static_assert(std::is_base_of<
                    std::forward_iterator_tag,
                    std::iterator_traits<ForwardIt>::iterator_category>::value,
                "Iterator needs to support forward access");

  auto count = std::distance(first, last);
  auto it = first;
  auto step = std::iterator_traits<ForwardIt>::difference_type{};
  while (count > 0) {
    step = count / 2;
    std::advance(it, step);

    if (comp(*it, value)) {
      count -= std::distance(first, it) + 1;
      first = it + 1;
    } else {
      count -= std::distance(it, last);
      last = it;
    }
    it = first;
  }

  return it;
}

template <class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value) {
  return STL::lower_bound(
      first, last, value,
      std::less<std::iterator_traits<ForwardIt>::value_type>{});
}

template <class ForwardIt, class T, class Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T &value,
                      Compare comp) {
  static_assert(std::is_base_of<
                    std::forward_iterator_tag,
                    std::iterator_traits<ForwardIt>::iterator_category>::value,
                "Iterator needs to support forward access");

  auto count = std::distance(first, last);
  auto it = first;
  auto step = count / 2;
  while (count > 0) {
    step = count / 2;
    std::advance(it, step);
    if (comp(value, *it)) {
      count -= std::distance(it, last);
      last = it;
    } else {
      count -= std::distance(first, it) + 1;
      first = it + 1;
    }
    it = first;
  }

  return it;
}

template <class ForwardIt, class T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T &value) {
  return STL::upper_bound(
      first, last, value,
      std::less<std::iterator_traits<ForwardIt>::value_type>{});
}

template <class ForwardIt, class T, class Compare>
bool binary_search(ForwardIt first, ForwardIt last, const T &value,
                   Compare comp) {
  static_assert(std::is_base_of<
                    std::forward_iterator_tag,
                    std::iterator_traits<ForwardIt>::iterator_category>::value,
                "Forward iterator required");

  auto lb = STL::lower_bound(first, last, value, comp);
  return !(lb == last) && !comp(*lb, value);
}

template <class ForwardIt, class T>
bool binary_search(ForwardIt first, ForwardIt last, const T &value) {
  return STL::binary_search(
      first, last, value,
      std::less<std::iterator_traits<ForwardIt>::value_type>{});
}

template <class ForwardIt, class T, class Compare>
std::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last,
                                            const T &value, Compare comp) {
  static_assert(std::is_base_of<
                    std::forward_iterator_tag,
                    std::iterator_traits<ForwardIt>::iterator_category>::value,
                "Forward iterator required");
  return {STL::lower_bound(first, last, value, comp),
          STL::upper_bound(first, last, value, comp)};
}

template <class ForwardIt, class T>
std::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last,
                                            const T &value) {
  return STL::equal_range(
      first, last, value,
      std::less<std::iterator_traits<ForwardIt>::value_types>{});
}

template <class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
               OutputIt d_first, Compare comp) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *d_first++ = *first1++;
    } else {
      *d_first++ = *first2++;
    }
  }
  while (first1 != last1) {
    *d_first++ = *first1++;
  }
  while (first2 != last2) {
    *d_first++ = *first2++;
  }

  return d_first;
}

template <class InputIt1, class InputIt2, class OutputIt>
OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
               OutputIt d_first) {
  return STL::merge(first1, last1, first2, last2, d_first,
                    [](auto &v1, auto &v2) { return v1 < v2; });
}

template <class BidirIt>
void inplace_merge(BidirIt first, BidirIt middle, BidirIt last) {
  STL::inplace_merge(first, middle, last,
                     std::less<std::iterator_traits<BidirIt>::value_type>{});
}

template <class BidirIt, class Compare>
void inplace_merge(BidirIt first, BidirIt middle, BidirIt last, Compare comp) {
  static_assert(
      std::is_base_of<std::bidirectional_iterator_tag,
                      std::iterator_traits<BidirIt>::iterator_category>::value,
      "Bidirectional iterator required");

  while (middle != last) {
    if (comp(*first, *middle)) {
      ++first;
    } else {
      first = STL::rotate(first, middle, middle + 1);
      ++middle;
    }
  }
}

template <class InputIt1, class InputIt2, class Compare>
bool includes(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
              Compare comp) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  if (first2 == last2)
    return true;

  while (first2 != last2 && first1 != last1) {
    if (comp(*first1++, *first2)) {
      ++first2;
    }
  }
  return first2 == last2;
}

template <class InputIt1, class InputIt2>
bool includes(InputIt1 first1, InputIt1 last1, InputIt2 first2,
              InputIt2 last2) {
  return STL::includes(
      first1, last1, first2, last2,
      [](const std::iterator_traits<InputIt1>::value_type &val1,
         const std::iterator_traits<InputIt2>::value_type &val2) {
        return val1 == val2;
      });
}

template <class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_difference(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                        InputIt2 last2, OutputIt d_first, Compare comp) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  while (first1 != last1) {
    if (first2 == last2) {
      d_first = STL::copy(first1, last1, d_first);
      break;
    }

    if (comp(*first1, *first2)) {
      *d_first++ = *first1++;
    } else {
      if (!comp(*first2, *first1)) {
        ++first1;
      } else {
        ++first2;
      }
    }
  }

  return d_first;
}

template <class InputIt1, class InputIt2, class OutputIt>
OutputIt set_difference(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                        InputIt2 last2, OutputIt d_first) {
  return STL::set_difference(
      first1, last1, first2, last2, d_first,
      [](const auto &val1, const auto &val2) { return val1 < val2; });
}

template <class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                          InputIt2 last2, OutputIt d_first, Compare comp) {

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      ++first1;
    } else {
      if (!comp(*first2, *first1)) {
        *d_first++ = *first1++;
      }
      ++first2;
    }
  }
  return d_first;
}

template <class InputIt1, class InputIt2, class OutputIt>
OutputIt set_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                          InputIt2 last2, OutputIt d_first) {
  return STL::set_intersection(
      first1, last1, first2, last2,
      d_first, [](const auto &v1, const auto &v2) noexcept { return v1 < v2; });
}

template <class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_symmetric_difference(InputIt1 first1, InputIt1 last1,
                                  InputIt2 first2, InputIt2 last2,
                                  OutputIt d_first, Compare comp) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *d_first++ = *first1++;
    } else {
      if (comp(*first2, *first1)) {
        *d_first++ = *first2++;
      } else {
        ++first1;
        ++first2;
      }
    }
  }
  d_first = STL::copy(first1, last1, d_first);
  d_first = STL::copy(first2, last2, d_first);
  return d_first;
}

template <class InputIt1, class InputIt2, class OutputIt>
OutputIt set_symmetric_difference(InputIt1 first1, InputIt1 last1,
                                  InputIt2 first2, InputIt2 last2,
                                  OutputIt d_first) {
  return STL::set_symmetric_difference(
      first1, last1, first2, last2, d_first,
      [](const auto &val1, const auto &val2) { return val1 < val2; });
}

template <class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt set_union(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                   InputIt2 last2, OutputIt d_first, Compare comp) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *d_first++ = *first1++;
    } else {
      *d_first++ = *first2;
      if (!comp(*first2, *first1)) {
        ++first1;
      }
      ++first2;
    }
  }
  d_first = STL::copy(first1, last1, d_first);
  d_first = STL::copy(first2, last2, d_first);
  return d_first;
}

template <class InputIt1, class InputIt2, class OutputIt>
OutputIt set_union(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                   InputIt2 last2, OutputIt d_first) {
  return STL::set_union(
      first1, last1, first2, last2, d_first,
      [](const auto &val1, const auto &val2) { return val1 < val2; });
}

template <class InputIt, class T, class BinaryOperation>
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");
  auto res = init;
  while (first != last) {
    res = op(res, *first++);
  }
  return res;
}

template <class InputIt, class T>
T accumulate(InputIt first, InputIt last, T init) {
  return STL::accumulate(
      first, last, init,
      [](const auto &val1, const auto &val2) { return val1 + val2; });
}

template <class InputIt1, class InputIt2, class T, class BinaryOperation1,
          class BinaryOperation2>
T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T value,
                BinaryOperation1 op1, BinaryOperation2 op2) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  auto acc = value;
  while (first1 != last1) {
    acc = op1(acc, op2(*first1++, *first2++));
  }
  return acc;
}

template <class InputIt1, class InputIt2, class T>
T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T value) {
  return STL::inner_product(
      first1, last1, first2, value,
      [](const auto &val1, const auto &val2) { return val1 + val2; },
      [](const auto &val1, const auto &val2) { return val1 * val2; });
}

template <class InputIt, class OutputIt, class BinaryOperation>
OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt d_first,
                             BinaryOperation op) {

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  auto prev = std::iterator_traits<InputIt>::value_type{};
  while (first != last) {
    prev = op(*first++, prev);
    *d_first++ = prev;
  }
  return d_first;
}

template <class InputIt, class OutputIt>
OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt d_first) {
  return STL::adjacent_difference(
      first, last, d_first,
      [](const auto &val1, const auto &val2) { return val1 - val2; });
}

template <class InputIt, class OutputIt, class BinaryOperation>
OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first,
                     BinaryOperation op) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");
  auto prev = std::iterator_traits<InputIt>::value_type{};
  while (first != last) {
    prev = op(prev, *first++);
    *d_first++ = prev;
  }
  return d_first;
}

template <class InputIt, class OutputIt>
OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first) {
  return STL::partial_sum(
      first, last, d_first,
      [](const auto &val1, const auto &val2) { return val1 + val2; });
}

template <class InputIt, class T, class BinaryOp>
T reduce(InputIt first, InputIt last, T init, BinaryOp binary_op) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_convertible<std::iterator_traits<InputIt>::value_type, T>::value,
      "incompatible T and InputIt");

  auto res = init;
  while (first != last) {
    res = binary_op(res, *first++);
  }
  return res;
}

template <class InputIt, class T>
T reduce(InputIt first, InputIt last, T init) {
  return STL::reduce(first, last, init, [](const auto &val1, const auto &val2) {
    return val1 + val2;
  });
}

template <class InputIt> auto reduce(InputIt first, InputIt last) {
  return STL::reduce(first, last, std::iterator_traits<InputIt>::value_type{});
}

template <class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init,
                        BinaryOperation binary_op) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  auto val = init;
  while (first != last) {
    *d_first++ = val;
    val = binary_op(val, *first++);
  }
  return d_first;
}

template <class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init) {
  return STL::exclusive_scan(first, last, d_first, init, std::plus<T>{});
}

template <class InputIt, class OutputIt, class BinaryOperation, class T>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first,
                        BinaryOperation binary_op, T init) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  auto val = init;
  while (first != last) {
    val = binary_op(val, *first++);
    *d_first++ = val;
  }
  return d_first;
}

template <class InputIt, class OutputIt, class BinaryOperation>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first,
                        BinaryOperation binary_op) {
  return STL::inclusive_scan(
      first, last, d_first, binary_op,
      typename std::iterator_traits<OutputIt>::value_type{});
}

template <class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first) {
  return STL::inclusive_scan(
      first, last, d_first,
      std::plus<typename std::iterator_traits<OutputIt>::value_type>{});
}

template <class InputIt, class T, class BinaryOp, class UnaryOp>
T transform_reduce(InputIt first, InputIt last, T init, BinaryOp binop,
                   UnaryOp unary_op) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  auto val = init;
  while (first != last) {
    val = binop(val, unary_op(*first++));
  }
  return val;
}

template <class InputIt1, class InputIt2, class T, class BinaryOp1,
          class BinaryOp2>
T transform_reduce(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init,
                   BinaryOp1 binary_op1, BinaryOp2 binary_op2) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  auto val = init;
  while (first1 != last1) {
    val = binary_op1(val, binary_op2(*first1++, *first2++));
  }
  return val;
}

template <class InputIt1, class InputIt2, class T>
T transform_reduce(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  return STL::transform_reduce(
      first1, last1, first2, init,
      std::plus<typename std::iterator_traits<InputIt1>::value_type>(),
      std::multiplies<typename std::iterator_traits<InputIt2>::value_type>());
}

template <class InputIt, class OutputIt, class T, class BinaryOperation,
          class UnaryOperation>
OutputIt transform_exclusive_scan(InputIt first, InputIt last, OutputIt d_first,
                                  T init, BinaryOperation binary_op,
                                  UnaryOperation unary_op) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  auto val = init;
  while (first != last) {
    *first = unary_op(*first);
    *d_first++ = val;
    val = binary_op(val, *first++);
  }
  return val;
}

template <class InputIt, class OutputIt, class BinaryOperation,
          class UnaryOperation, class T>
OutputIt transform_inclusive_scan(InputIt first, InputIt last, OutputIt d_first,
                                  BinaryOperation binary_op,
                                  UnaryOperation unary_op, T init) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt>::iterator_category>::value,
      "Input iterator required");

  auto val = init;
  while (first != last) {
    *first = unary_op(*first);
    val = binary_op(val, *first++);
    *d_first++ = val;
  }
  return val;
}

template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt1>::iterator_category>::value,
      "Input iterator required");

  static_assert(
      std::is_base_of<std::input_iterator_tag,
                      std::iterator_traits<InputIt2>::iterator_category>::value,
      "Input iterator required");

  while (first1 != last1) {
    if (comp(*first1, *first2)) {
      return true;
    } else {
      if (comp(*first2, *first1)) {
        return false;
      } else {
        ++first1;
        ++first2;
      }
    }
  }
  return false;
}

template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
  return STL::lexicographical_compare(
      first1, last1, first2, last2,
      std::less<typename std::iterator_traits<InputIt1>::value_type>());
}

} // namespace STL