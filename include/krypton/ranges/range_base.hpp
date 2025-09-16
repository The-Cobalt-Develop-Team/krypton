//
// Created by Renatus Madrigal on 09/16/2025
//

#ifndef KRYPTON_INCLUDE_KRYPTON_COMMON_RANGES_RANGE_BASE_HPP_
#define KRYPTON_INCLUDE_KRYPTON_COMMON_RANGES_RANGE_BASE_HPP_

#ifdef KRYPTON_USE_STD_RANGES
#include <ranges>
#else
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_fwd.hpp>

#endif

namespace krypton {

namespace ranges {

#ifdef KRYPTON_USE_STD_RANGES
using namespace std::ranges;
#else
using namespace ::ranges;

// The `std::ranges::view` concept is equivalent to `ranges::view_` in range-v3.
template <typename Rng>
concept view = ranges::view_<Rng>;

#endif

namespace views {

#ifdef KRYPTON_USE_STD_RANGES
using namespace std::ranges::views;
#else
using namespace ::ranges::views;
#endif

} // namespace views

} // namespace ranges

namespace views = ranges::views; // NOLINT

} // namespace krypton

#endif // KRYPTON_INCLUDE_KRYPTON_COMMON_RANGES_RANGE_BASE_HPP_