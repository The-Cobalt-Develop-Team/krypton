//
// Created by Renatus Madrigal on 09/09/2025
//

#pragma once

#ifndef KRYPTON_INCLUDE_KRYPTON_COMMON_CONCEPT_HPP_
#define KRYPTON_INCLUDE_KRYPTON_COMMON_CONCEPT_HPP_

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace krypton {

template <typename T> struct is_byte_like {
  // We assume that all byte-like types are convertible to uint8_t except std::byte.
  constexpr static bool value =
      sizeof(T) == 1 && std::is_convertible_v<T, uint8_t> || std::is_same_v<T, std::byte>;
};

template <typename T> constexpr bool is_byte_like_v = is_byte_like<T>::value;

template <typename T>
concept byte_like = requires { is_byte_like_v<T>; };

// These type should be byte_like in almost all platform.
static_assert(is_byte_like_v<char> && is_byte_like_v<unsigned char> && is_byte_like_v<uint8_t>);
static_assert(is_byte_like_v<std::byte>);

// Pointers are not supposed to be byte_like.
static_assert(!is_byte_like_v<std::nullptr_t>);
static_assert(!is_byte_like_v<int *>);

// Obviously not byte_like.
static_assert(!is_byte_like_v<int64_t> && !is_byte_like_v<uint64_t>);
static_assert(!is_byte_like_v<float> && !is_byte_like_v<double>);

} // namespace krypton

#endif // KRYPTON_INCLUDE_KRYPTON_COMMON_CONCEPT_HPP_
