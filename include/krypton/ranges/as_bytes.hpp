//
// Created by Renatus Madrigal on 09/16/2025
//

#ifndef KRYPTON_INCLUDE_KRYPTON_COMMON_RANGES_AS_BYTES_HPP_
#define KRYPTON_INCLUDE_KRYPTON_COMMON_RANGES_AS_BYTES_HPP_

#include "krypton/common/common.hpp"
#include "krypton/ranges/range_base.hpp"
#include <iterator>

#ifndef KRYPTON_USE_STD_RANGES
#include <range/v3/view/all.hpp>
#endif

namespace krypton::ranges {

namespace ext {
namespace detail {
template <typename BaseIter> class contiguous_as_byte_iterator {
  // Currently the implementation of contiguous_as_byte_iterator is almost the same as the
  // noncontiguous version. We keep them separate for clarity and potential future optimizations.
private:
  BaseIter current_;
  std::size_t index_ = 0;

public:
  // It should be a random access iterator, but now we do not implement the operator[], so we
  // downgrade it to bidirectional.
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type        = byte;
  using difference_type   = std::ptrdiff_t;
  using pointer           = void; // The proxy type does not support operator->.
  using reference         = byte; // The proxy type returns by value.

  contiguous_as_byte_iterator() = default;

  explicit contiguous_as_byte_iterator(BaseIter current, std::size_t index = 0)
      : current_(current), index_(index) {}

  reference operator*() const {
    auto ptr = reinterpret_cast<const byte *>(std::to_address(current_));
    return ptr[index_];
  }

  contiguous_as_byte_iterator &operator++() {
    ++index_;
    if (index_ == sizeof(*current_)) {
      index_ = 0;
      ++current_;
    }
    return *this;
  }

  contiguous_as_byte_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  contiguous_as_byte_iterator &operator--() {
    if (index_ == 0) {
      --current_;
      index_ = sizeof(*current_);
    }
    --index_;
    return *this;
  }

  contiguous_as_byte_iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  contiguous_as_byte_iterator &operator+=(difference_type diff) {
    // The number of bytes in one element of the underlying range.
    const auto element_size = sizeof(*current_);

    // This can be negative, so use a signed type.
    const difference_type total_offset = index_ + diff;

    // How many full elements we need to advance (or retreat).
    const auto element_offset = total_offset / element_size;

    // The new byte index within the target element.
    // The modulo operator in C++ can yield a negative result for a negative input.
    // We adjust it to ensure the index is always a valid, non-negative offset.
    index_ = total_offset % element_size;
    if (index_ < 0) {
      index_ += element_size;
      // We borrowed from the previous element, so adjust the element offset.
      current_ += element_offset - 1;
    } else {
      current_ += element_offset;
    }

    return *this;
  }

  contiguous_as_byte_iterator &operator-=(difference_type diff) { return *this += -diff; }

  friend contiguous_as_byte_iterator operator+(contiguous_as_byte_iterator iter,
                                               difference_type n) {
    return iter += n;
  }
  friend contiguous_as_byte_iterator operator+(difference_type n,
                                               contiguous_as_byte_iterator iter) {
    return iter += n;
  }
  friend contiguous_as_byte_iterator operator-(contiguous_as_byte_iterator iter,
                                               difference_type n) {
    return iter -= n;
  }

  friend difference_type operator-(contiguous_as_byte_iterator lhs,
                                   contiguous_as_byte_iterator rhs) {
    const auto elem_diff = lhs.current_ - rhs.current_;
    const auto byte_diff = lhs.index_ - rhs.index_;
    return elem_diff * sizeof(*current_) + byte_diff;
  }

  friend bool operator==(contiguous_as_byte_iterator lhs,
                         contiguous_as_byte_iterator rhs) = default;

  friend auto operator<=>(contiguous_as_byte_iterator lhs, contiguous_as_byte_iterator rhs) {
    return lhs.current_ == rhs.current_ ? lhs.index_ <=> rhs.index_ : lhs.current_ <=> rhs.current_;
  }
};

template <typename BaseIter> class noncontiguous_as_byte_iterator {
  static_assert(ranges::input_iterator<BaseIter>,
                "The base iterator must satisfy the input iterator concept");
  static_assert(std::is_copy_assignable_v<typename std::iterator_traits<BaseIter>::value_type>,
                "The value type of the base iterator must be copy assignable");

private:
  BaseIter current_;
  std::size_t index_ = 0;

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type        = byte;
  using difference_type   = std::ptrdiff_t;
  using pointer           = void; // The proxy type does not support operator->.
  using reference         = byte; // The proxy type returns by value.

  noncontiguous_as_byte_iterator() = default;

  explicit noncontiguous_as_byte_iterator(BaseIter current, std::size_t index = 0)
      : current_(current), index_(index) {}

  reference operator*() const {
    auto res = reinterpret_cast<const byte *>(&(*current_))[index_];
    return res;
  }

  noncontiguous_as_byte_iterator &operator++() {
    ++index_;
    if (index_ == sizeof(*current_)) {
      index_ = 0;
      ++current_;
    }
    return *this;
  }

  noncontiguous_as_byte_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  noncontiguous_as_byte_iterator &operator--() {
    if (index_ == 0) {
      --current_;
      index_ = sizeof(*current_);
    }
    --index_;
    return *this;
  }

  noncontiguous_as_byte_iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  noncontiguous_as_byte_iterator &operator+=(difference_type diff) {
    const auto element_size            = sizeof(*current_);
    const difference_type total_offset = index_ + diff;
    const auto element_offset          = total_offset / element_size;

    index_ = total_offset % element_size;
    if (index_ < 0) {
      index_ += element_size;
      current_ += element_offset - 1;
    } else {
      current_ += element_offset;
    }

    return *this;
  }

  noncontiguous_as_byte_iterator &operator-=(difference_type diff) { return *this += -diff; }

  friend noncontiguous_as_byte_iterator operator+(noncontiguous_as_byte_iterator iter,
                                                  difference_type n) {
    return iter += n;
  }
  friend noncontiguous_as_byte_iterator operator+(difference_type n,
                                                  noncontiguous_as_byte_iterator iter) {
    return iter += n;
  }
  friend noncontiguous_as_byte_iterator operator-(noncontiguous_as_byte_iterator iter,
                                                  difference_type n) {
    return iter -= n;
  }

  friend difference_type operator-(noncontiguous_as_byte_iterator lhs,
                                   noncontiguous_as_byte_iterator rhs) {
    const auto elem_diff = lhs.current_ - rhs.current_;
    const auto byte_diff = lhs.index_ - rhs.index_;
    return elem_diff * sizeof(*current_) + byte_diff;
  }

  friend bool operator==(noncontiguous_as_byte_iterator lhs,
                         noncontiguous_as_byte_iterator rhs) = default;

  friend auto operator<=>(noncontiguous_as_byte_iterator lhs, noncontiguous_as_byte_iterator rhs) {
    return lhs.current_ == rhs.current_ ? lhs.index_ <=> rhs.index_ : lhs.current_ <=> rhs.current_;
  }
};

} // namespace detail

template <ranges::input_range Rng>
// requires ranges::contiguous_range<Rng>
class as_bytes_view : public ranges::view_interface<as_bytes_view<Rng>> {
  static_assert(std::is_trivially_copyable_v<ranges::range_value_t<Rng>>,
                "as_bytes_view requires a range of trivially copyable types");

private:
  Rng base_;
  using BaseIter = ranges::iterator_t<Rng>;

public:
  using iterator = std::conditional_t<ranges::contiguous_range<Rng>,
                                      detail::contiguous_as_byte_iterator<BaseIter>,
                                      detail::noncontiguous_as_byte_iterator<BaseIter>>;

  as_bytes_view() = default;

  explicit as_bytes_view(Rng rng) : base_(std::move(rng)) {}

  auto begin() { return iterator(ranges::begin(base_)); }

  auto end() { return iterator(ranges::end(base_)); }

  auto size() { return ranges::size(base_) * sizeof(ranges::range_value_t<Rng>); }
};

} // namespace ext

namespace views::ext {

struct as_bytes_fn {
  template <ranges::viewable_range Rng> view auto operator()(Rng &&rng) const {
    return ranges::ext::as_bytes_view(views::all(std::forward<Rng>(rng)));
  }

  friend constexpr auto operator|(ranges::viewable_range auto &&rng, as_bytes_fn) {
    return as_bytes_fn{}(std::forward<decltype(rng)>(rng));
  }
};

inline constexpr as_bytes_fn as_bytes{};

} // namespace views::ext

static_assert(ranges::view<ranges::ext::as_bytes_view<std::vector<int>>>);

} // namespace krypton::ranges

#endif // KRYPTON_INCLUDE_KRYPTON_COMMON_RANGES_AS_BYTES_HPP_