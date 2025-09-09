//
// Created by Renatus Madrigal on 09/09/2025
//

#include "krypton/common/secure_vector.hpp"

#include <gtest/gtest.h>
#include <memory_resource>

using namespace krypton;

TEST(KryptonCommon, SecureVectorTest1) {
  secure_vector<int> vec;
  vec.push_back(42);
  vec.push_back(13);
  vec.push_back(7);
  EXPECT_EQ(vec.size(), 3);
  vec.clear();
  EXPECT_EQ(vec.size(), 0);
}

TEST(KryptonCommon, SecureVectorTest2) {
  char buffer[1024];
  char *ptr = nullptr;
  std::pmr::monotonic_buffer_resource pool{buffer, sizeof(buffer)};
  {
    pmr::secure_vector<char> vec{&pool};
    vec.resize(10, 'A');
    EXPECT_EQ(vec[0], 'A');
    ptr = vec.data();
    EXPECT_EQ(*ptr, 'A');
  }
  EXPECT_EQ(*ptr, 0);
}
