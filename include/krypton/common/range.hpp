//
// Created by Renatus Madrigal on 09/09/2025
//

#ifndef KRYPTON_INCLUDE_KRYPTON_COMMON_RANGE_HPP_
#define KRYPTON_INCLUDE_KRYPTON_COMMON_RANGE_HPP_

#include "common.hpp"
#include <cstddef>
#include <iterator>

#ifdef KRYPTON_USE_STD_RANGES
#include <ranges>
#else
#include <range/v3/range.hpp>
#include <range/v3/range/access.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/range/traits.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/interface.hpp>
#include <range/v3/view/view.hpp>
#endif
#include <type_traits>

namespace krypton {

namespace ranges {

#ifdef KRYPTON_USE_STD_RANGES
using namespace std::ranges;
#else
using namespace ::ranges;

// The `std::ranges::view` concept is equivalent to `ranges::view_` in range-v3.
// We define it here for consistency.
template <typename Rng>
concept view = ranges::view_<Rng>;

#endif

namespace views {

#ifdef KRYPTON_USE_STD_RANGES
using namespace std::ranges::views;
#else
using namespace ::ranges::views;
#endif

namespace ext {

template <ranges::input_range Rng>
  requires ranges::contiguous_range<Rng>
class as_bytes_view : public ranges::view_interface<as_bytes_view<Rng>> {
  static_assert(std::is_trivially_copyable_v<ranges::range_value_t<Rng>>,
                "as_bytes_view requires a range of trivially copyable types");

private:
  Rng base_;
  using BaseIter = ranges::iterator_t<Rng>;

public:
  class iterator {
  private:
    BaseIter current_;
    std::size_t index_ = 0;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = byte;
    using difference_type   = std::ptrdiff_t;
    using pointer           = void; // The proxy type does not support operator->.
    using reference         = byte; // The proxy type returns by value.

    iterator() = default;

    explicit iterator(BaseIter current, std::size_t index = 0) : current_(current), index_(index) {}

    reference operator*() const {
      auto ptr = reinterpret_cast<const byte *>(std::to_address(current_));
      return ptr[index_];
    }

    iterator &operator++() {
      ++index_;
      if (index_ == sizeof(*current_)) {
        index_ = 0;
        ++current_;
      }
      return *this;
    }

    iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    iterator &operator--() {
      if (index_ == 0) {
        --current_;
        index_ = sizeof(*current_);
      }
      --index_;
      return *this;
    }

    iterator operator--(int) {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    iterator &operator+=(difference_type diff) {
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

    iterator &operator-=(difference_type diff) { return *this += -diff; }

    friend iterator operator+(iterator iter, difference_type n) { return iter += n; }
    friend iterator operator+(difference_type n, iterator iter) { return iter += n; }
    friend iterator operator-(iterator iter, difference_type n) { return iter -= n; }

    friend difference_type operator-(iterator lhs, iterator rhs) {
      const auto elem_diff = lhs.current_ - rhs.current_;
      const auto byte_diff = lhs.index_ - rhs.index_;
      return elem_diff * sizeof(*current_) + byte_diff;
    }

    friend bool operator==(iterator lhs, iterator rhs) = default;

    friend auto operator<=>(iterator lhs, iterator rhs) {
      return lhs.current_ == rhs.current_ ? lhs.index_ <=> rhs.index_
                                          : lhs.current_ <=> rhs.current_;
    }
  };

  as_bytes_view() = default;

  explicit as_bytes_view(Rng rng) : base_(std::move(rng)) {}

  auto begin() { return iterator(ranges::begin(base_)); }

  auto end() { return iterator(ranges::end(base_)); }

  auto size() { return ranges::size(base_) * sizeof(ranges::range_value_t<Rng>); }
};

struct as_bytes_fn {
  template <ranges::viewable_range Rng> view auto operator()(Rng &&rng) const {
    return as_bytes_view(views::all(std::forward<Rng>(rng)));
  }

  friend constexpr auto operator|(ranges::viewable_range auto &&rng, as_bytes_fn) {
    return as_bytes_fn{}(std::forward<decltype(rng)>(rng));
  }
};

inline constexpr as_bytes_fn as_bytes{};

} // namespace ext

static_assert(ranges::view<ext::as_bytes_view<std::vector<int>>>);

} // namespace views

} // namespace ranges

namespace views = ranges::views; // NOLINT

} // namespace krypton

#endif // KRYPTON_INCLUDE_KRYPTON_COMMON_RANGE_HPP_