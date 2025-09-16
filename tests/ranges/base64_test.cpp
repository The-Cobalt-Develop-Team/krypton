//
// Created by Renatus Madrigal on 09/16/2025
//

#include "krypton/ranges/base64.hpp"
#include <gtest/gtest.h>
#include <range/v3/range/conversion.hpp>
#include <string>

using namespace krypton;
namespace ext_rng = krypton::ranges::ext;
using namespace std::string_view_literals;

TEST(RangeTest, Base64EncodeTest) {
  std::string input           = "Yoimiya!";
  std::string expected_output = "WW9pbWl5YSE=";
  std::string output          = ext_rng::base64_encode(input) | ::ranges::to<std::string>();
  EXPECT_EQ(expected_output, output);
}

TEST(RangeTest, Base64EncodeEmptyTest) {
  std::string input           = "";
  std::string expected_output = "";
  std::string output          = ext_rng::base64_encode(input) | ::ranges::to<std::string>();
  EXPECT_EQ(expected_output, output);
}

TEST(RangeTest, Base64EncodeShortTest) {
  std::string input           = "Hi";
  std::string expected_output = "SGk=";
  std::string output          = ext_rng::base64_encode(input) | ::ranges::to<std::string>();
  EXPECT_EQ(expected_output, output);
}

TEST(RangeTest, Base64EncodeCustomPaddingTest) {
  constexpr auto custom_alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/-"sv;
  auto encoder                = ext_rng::make_base64_encoder(custom_alphabet);
  std::string input           = "Yoimiya!";
  std::string expected_output = "WW9pbWl5YSE-";
  std::string output          = encoder(input) | ::ranges::to<std::string>();
  EXPECT_EQ(expected_output, output);
}

TEST(RangeTest, Base64EncodeCustomAlphabetTest) {
  constexpr auto custom_alphabet =
      "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba9876543210+/="sv;
  auto encoder                = ext_rng::make_base64_encoder(custom_alphabet);
  std::string input           = "Yoimiya!";
  std::string expected_output = "DD0kyDo4BHV=";
  std::string output          = encoder(input) | ::ranges::to<std::string>();
  EXPECT_EQ(expected_output, output);
}

TEST(RangeTest, Base64EncodeToVectorByteTest) {
  std::string input           = "Yoimiya!";
  std::string expected_output = "WW9pbWl5YSE=";
  auto output                 = ext_rng::base64_encode(input) | ::ranges::to<std::vector<byte>>();
  std::string output_str(output.begin(), output.end());
  EXPECT_EQ(expected_output, output_str);
}