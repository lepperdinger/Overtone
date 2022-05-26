/******************************************************************************

    Overtone: A Music Visualizer

    test_KeyboardFrequencies.cpp

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

#include "KeyboardFrequencies.h"
#include <algorithm>
#include <gtest/gtest.h>

TEST(test_KeyboardFrequencies, key_frequency_conversion) {
  double tolerance{1e-12};
  double frequency{440};
  double key{48};
  EXPECT_NEAR(KeyboardFrequencies::key_to_frequency(key), frequency, tolerance);
  EXPECT_NEAR(KeyboardFrequencies::frequency_to_key(frequency), key, tolerance);

  frequency = 34.55235;
  EXPECT_NEAR(KeyboardFrequencies::key_to_frequency(
                  KeyboardFrequencies::frequency_to_key(frequency)),
              frequency, tolerance);
}

TEST(test_KeyboardFrequencies, key_range_to_frequency_range) {
  using Vector = std::vector<double>;
  using VectorSize = Vector::size_type;
  using VectorRange = std::pair<VectorSize, VectorSize>;

  std::vector<double> keys{22.3, 22.6, 23.3, 28, 30, 44.3};
  std::pair<VectorSize, VectorSize> key_range{23, 32};
  std::vector<double> frequencies;
  std::transform(keys.cbegin(), keys.cend(), std::back_inserter(frequencies),
                 KeyboardFrequencies::key_to_frequency);
  VectorRange frequency_range =
      KeyboardFrequencies::key_range_to_frequency_range(key_range, frequencies);
  VectorRange expected{1, 5};
  EXPECT_EQ(frequency_range, expected);
}
