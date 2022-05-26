/******************************************************************************

    Overtone: A Music Visualizer

    KeyboardFrequencies.h

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

#ifndef OVERTONE_SRC_KEYBOARDFREQUENCIES_H
#define OVERTONE_SRC_KEYBOARDFREQUENCIES_H

#include <vector>

class KeyboardFrequencies {
public:
  /**
   * Converts a key to a frequency.
   * @param key key
   * @return frequency
   */
  static double key_to_frequency(const double &key);

  /**
   * Converts a frequency to a key.
   * @param frequency frequency
   * @return key
   */
  static double frequency_to_key(const double &frequency);

  /**
   * Converts a vector of frequencies to a vector of keys.
   * @param frequencies frequencies
   * @return keys
   */
  static std::vector<double>
  frequencies_to_keys(const std::vector<double> &frequencies);

  /**
   * Returns the index range of the frequencies between the key range
   * key_range.first - 0.5 and key_range.second + 0.5
   * @param key_range selected key range
   * @param all_frequencies all frequencies of the Fourier transform
   * @return frequencies that contain the specified key range
   */
  static std::pair<std::vector<double>::size_type,
                   std::vector<double>::size_type>
  key_range_to_frequency_range(
      const std::pair<unsigned char, unsigned char> &key_range,
      const std::vector<double> &all_frequencies);
};

#endif // OVERTONE_SRC_KEYBOARDFREQUENCIES_H