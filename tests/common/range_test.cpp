//
// Created by Renatus Madrigal on 09/14/2025
//

#include "krypton/common/range.hpp"
#include <gtest/gtest.h>
#include <range/v3/range/concepts.hpp>

using namespace krypton;

TEST(RangeTest, AsBytesViewTest) {
  std::vector<int> vec = {1, 2, 3, 4};
  auto byte_view       = views::ext::as_bytes(vec);

  std::vector<uint8_t> expected_bytes;
  for (int num : vec) {
    uint8_t *byte_ptr = reinterpret_cast<uint8_t *>(&num);
    expected_bytes.insert(expected_bytes.end(), byte_ptr, byte_ptr + sizeof(int));
  }

  std::vector<uint8_t> actual_bytes(byte_view.begin(), byte_view.end());
  EXPECT_EQ(actual_bytes, expected_bytes);
}

TEST(RangeTest, AsBytesViewEmptyTest) {
  std::vector<int> vec = {};
  auto byte_view       = views::ext::as_bytes(vec);
  EXPECT_TRUE(byte_view.begin() == byte_view.end());
  EXPECT_EQ(std::distance(byte_view.begin(), byte_view.end()), 0);
}

TEST(RangeTest, AsBytesViewPipeTest) {
  std::vector<int> vec = {5, 10, 15};
  auto actual_bytes    = vec | views::ext::as_bytes | ::ranges::to<std::vector<uint8_t>>();

  std::vector<uint8_t> expected_bytes;
  for (int num : vec) {
    uint8_t *byte_ptr = reinterpret_cast<uint8_t *>(&num);
    expected_bytes.insert(expected_bytes.end(), byte_ptr, byte_ptr + sizeof(int));
  }

  EXPECT_EQ(actual_bytes, expected_bytes);
}
