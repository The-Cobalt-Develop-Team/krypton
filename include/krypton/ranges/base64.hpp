//
// Created by Renatus Madrigal on 09/16/2025
//

#ifndef KRYPTON_INCLUDE_KRYPTON_RANGES_BASE64_HPP_
#define KRYPTON_INCLUDE_KRYPTON_RANGES_BASE64_HPP_

#include "krypton/common/common.hpp"
#include "krypton/ranges/range_base.hpp"
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/operations.hpp>
#include <range/v3/range/traits.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <utility>

namespace krypton::ranges::ext {

template <typename Rng>
  requires ranges::random_access_range<Rng>
constexpr auto make_base64_encoder(Rng &&table) {
  // FIXME: This cannot handle raw char* tables but not sure why. It neither works with non-const.
  return [table](auto &&input_range) {
    return input_range | views::chunk(3) |
           views::transform([table = std::forward<Rng>(table)](auto &&chunk) {
             std::array<byte, 3> bytes{0, 0, 0};
             for (auto &&[idx, byte] : views::enumerate(chunk)) {
               bytes[idx] = byte;
             }

             const byte b0 = (bytes[0] & 0b11111100) >> 2;
             const byte b1 = ((bytes[0] & 0b00000011) << 4) | ((bytes[1] & 0b11110000) >> 4);
             const byte b2 = ((bytes[1] & 0b00001111) << 2) | ((bytes[2] & 0b11000000) >> 6);
             const byte b3 = bytes[2] & 0b00111111;

             using char_type         = typename ranges::range_value_t<Rng>;
             const char_type padding = ranges::at(table, 64); // padding character
             std::array<char_type, 4> encoded{ranges::at(table, b0), ranges::at(table, b1),
                                              chunk.size() > 1 ? ranges::at(table, b2) : padding,
                                              chunk.size() > 2 ? ranges::at(table, b3) : padding};
             return encoded;
           }) |
           views::join;
  };
}

// We save the padding character '=' at the end to simplify indexing.
constexpr static std::string_view base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                 "abcdefghijklmnopqrstuvwxyz"
                                                 "0123456789+/=";

inline constexpr auto base64_encode = make_base64_encoder(base64_table);

} // namespace krypton::ranges::ext

#endif // KRYPTON_INCLUDE_KRYPTON_RANGES_BASE64_HPP_