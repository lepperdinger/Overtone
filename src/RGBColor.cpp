/******************************************************************************

    Overtone: A Music Visualizer

    RGBColor.cpp

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

std::vector<unsigned char>
RGBColor::hex_string_to_numbers(const std::string &hex_string) {
  std::vector<unsigned char> rgb_values;
  int base{16};
  for (size_t color_index = 0; color_index != 3; ++color_index) {
    size_t string_index = color_index * 2;
    std::string sub_string = hex_string.substr(string_index, 2);
    unsigned char rgb_value = std::strtoul(sub_string.c_str(), nullptr, base);
    rgb_values.push_back(rgb_value);
  }
  return rgb_values;
}
