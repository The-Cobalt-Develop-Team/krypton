//
// Created by Renatus Madrigal on 09/09/2025
//

#pragma once

#ifndef KRYPTON_INCLUDE_KRYPTON_COMMON_COMMON_HPP_
#define KRYPTON_INCLUDE_KRYPTON_COMMON_COMMON_HPP_

#include "concept.hpp"
#include <cstdint>

namespace krypton {

using byte = uint8_t;

template <typename T>
  requires byte_like<T>
constexpr byte to_byte(T val) {
  return static_cast<byte>(val);
}

} // namespace krypton

#endif // KRYPTON_INCLUDE_KRYPTON_COMMON_COMMON_HPP_