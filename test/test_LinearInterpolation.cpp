/******************************************************************************

    Overtone: A Music Visualizer

    test_LinearInterpolation.cpp

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

#include "LinearInterpolation.h"
#include <gtest/gtest.h>

TEST(test_LinearInterpolation, interpolate) {
  double tolerance = 1E-10;
  double expected = 1199.452154999999948;
  double result = LinearInterpolation::interpolate(
      {32.4, 857.816654999999969}, {132.43, 5183.614004999999452}, 40.3);
  EXPECT_NEAR(expected, result, tolerance);
}
