//
// Created by Renatus Madrigal on 09/09/2025
//

#pragma once

#ifndef KRYPTON_INCLUDE_KRYPTON_COMMON_SECURE_VECTOR_HPP_
#define KRYPTON_INCLUDE_KRYPTON_COMMON_SECURE_VECTOR_HPP_

#include <memory_resource>
#include <vector>

namespace krypton {

/**
 * @brief A vector that securely zeroes its contents upon destruction.
 */
template <typename T, typename Alloc = std::allocator<T>>
class secure_vector : public std::vector<T, Alloc> {
  // It may not be a good idea to just inherit from std::vector, but this may keep things
  // simple for now.
public:
  using std::vector<T, Alloc>::vector;

  ~secure_vector() {
    if constexpr (std::is_trivially_destructible_v<T> && !std::is_pointer_v<T>) {
      if (!this->empty()) {
        // For trivially destructible types (like int, char, float, or simple structs),
        // we can safely use memset to zero out the memory.
        // We avoid this for pointer types to prevent just zeroing the pointer
        // without freeing the memory it points to.
        memset(this->data(), 0, this->capacity() * sizeof(T));
      }
    } else {
      // For complex types (like std::string, or classes with destructors),
      // we must overwrite element by element to ensure destructors are
      // called correctly and resources are not leaked.
      std::fill(this->begin(), this->end(), T{});
    }
  }
};

namespace pmr {
template <typename T>
using secure_vector = krypton::secure_vector<T, std::pmr::polymorphic_allocator<T>>;
}

} // namespace krypton

#endif // KRYPTON_INCLUDE_KRYPTON_COMMON_SECURE_VECTOR_HPP_
