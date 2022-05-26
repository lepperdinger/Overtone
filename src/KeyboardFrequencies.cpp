/******************************************************************************

    Overtone: A Music Visualizer

    KeyboardFrequencies.cpp

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
#include <cmath>

double KeyboardFrequencies::key_to_frequency(const double &key) {
  return pow(2., (key - 48.) / 12.) * 440.;
}

double KeyboardFrequencies::frequency_to_key(const double &frequency) {
  return 48. + 12. * log2(frequency / 440.);
}

std::vector<double> KeyboardFrequencies::frequencies_to_keys(
    const std::vector<double> &frequencies) {
  std::vector<double> keys;
  keys.reserve(frequencies.size());
  std::transform(frequencies.cbegin(), frequencies.cend(),
                 std::back_inserter(keys), frequency_to_key);
  return keys;
}

std::pair<std::vector<double>::size_type, std::vector<double>::size_type>
KeyboardFrequencies::key_range_to_frequency_range(
    const std::pair<unsigned char, unsigned char> &key_range,
    const std::vector<double> &all_frequencies) {
  double lower_frequency =
      KeyboardFrequencies::key_to_frequency(key_range.first - 0.5);
  double upper_frequency =
      KeyboardFrequencies::key_to_frequency(key_range.second - 0.5);
  auto lower_iterator =
      std::find_if(all_frequencies.cbegin(), all_frequencies.cend(),
                   [&lower_frequency](const double &frequency) {
                     return lower_frequency < frequency;
                   });
  auto upper_iterator =
      std::find_if(all_frequencies.cbegin(), all_frequencies.cend(),
                   [&upper_frequency](const double &frequency) {
                     return upper_frequency < frequency;
                   });
  auto lower_index = lower_iterator - all_frequencies.cbegin();
  auto upper_index = upper_iterator - all_frequencies.cbegin();
  return {lower_index, upper_index};
}
