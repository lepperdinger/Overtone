/******************************************************************************

    Overtone: A Music Visualizer

    test_RGBColor.cpp

    Copyright (C) 2022  Stefan Lepperdinger

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

******************************************************************************/

#include "RGBColor.h"
#include <gtest/gtest.h>

TEST(test_RGBColor, hex_string_to_numbers) {
  std::vector<unsigned char> expected{0xff, 0xff, 0xff};
  EXPECT_EQ(RGBColor::hex_string_to_numbers("ffffff"), expected);
  EXPECT_EQ(RGBColor::hex_string_to_numbers("FFFFFF"), expected);
  EXPECT_EQ(RGBColor::hex_string_to_numbers("FfFffF"), expected);

  expected = {0x0, 0x0, 0x0};
  EXPECT_EQ(RGBColor::hex_string_to_numbers("000000"), expected);

  expected = {0xab, 0x51, 0x22};
  EXPECT_EQ(RGBColor::hex_string_to_numbers("ab5122"), expected);
}