/******************************************************************************

    Overtone: A Music Visualizer

    ColorMap.cpp

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

#include "ColorMap.h"
#include "LinearInterpolation.h"
#include "RGBColor.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

ColorMap::ColorMap() : gain(1.), gate(0.) { initialize_themes(); }

ColorMap::ColorMap(std::string theme, double gain, double gate)
    : gain(gain), gate(gate), theme(std::move(theme)) {
  if (gain < 0) {
    throw std::out_of_range("The argument `gain` is negative.");
  }
  if (gate < 0 || gate > 1) {
    throw std::out_of_range(
        "The argument `gate` is not within the interval 0 <= gate <= 1.");
  }
  initialize_themes();
  bool theme_exists = check_if_theme_exists(this->theme);
  if (!theme_exists) {
    auto theme_names = get_theme_names();
    std::stringstream message;
    message << "Overtone: Error: Theme '" << this->theme
            << "' not found. Available themes:\n";
    for (auto theme_name = theme_names.cbegin();
         theme_name != theme_names.cend(); ++theme_name) {
      message << "    -> " << *theme_name;
      if (theme_name != theme_names.cend() - 1) {
        message << '\n';
      }
    }
    throw std::invalid_argument(message.str());
  }
  determine_limits();
}

bool ColorMap::check_if_theme_exists(const std::string &theme_name) {
  bool color_map_exists = color_maps.find(theme_name) != color_maps.end();
  bool edge_color_exists = edge_colors.find(theme_name) != edge_colors.end();
  bool theme_exists = color_map_exists && edge_color_exists;
  return theme_exists;
}

void ColorMap::initialize_themes() {
  edge_colors["gray"] = RGBColor::hex_string_to_numbers("000000");
  color_maps["gray"] = convert_color_map({"202020", "ffffff"});

  edge_colors["white"] = RGBColor::hex_string_to_numbers("202020");
  color_maps["white"] = convert_color_map({"ffffff", "000000"});

  edge_colors["matrix"] = RGBColor::hex_string_to_numbers("003000");
  color_maps["matrix"] = convert_color_map({"000000", "00ff00"});

  edge_colors["cyan"] = RGBColor::hex_string_to_numbers("000000");
  color_maps["cyan"] =
      convert_color_map({"202020", "349d8c", "7ce1d2", "dcdadb"});

  edge_colors["fire"] = RGBColor::hex_string_to_numbers("000000");
  color_maps["fire"] =
      convert_color_map({"20030d", "32021b", "6f0511", "980506", "cb0503",
                         "df3405", "eb6001", "f78c01", "f6a805", "febb08"});

  edge_colors["vintage"] = RGBColor::hex_string_to_numbers("000000");
  color_maps["vintage"] =
      convert_color_map({"231f22", "777b62", "999a7c", "dbaa93", "efdfa4",
                         "facd66", "d08e5f", "9b7e68"});

  edge_colors["purple"] = RGBColor::hex_string_to_numbers("000000");
  color_maps["purple"] =
      convert_color_map({"0e042c", "230044", "3c076c", "581d96", "7d2cbc",
                         "a050df", "c57ffa", "e0adfb"});
}

std::vector<std::string> ColorMap::get_theme_names() {
  std::vector<std::string> theme_names;
  theme_names.reserve(color_maps.size());
  for (const auto &color_map : color_maps) {
    std::string theme_name(color_map.first);
    bool edge_color_exists = edge_colors.find(this->theme) != edge_colors.end();
    if (!edge_color_exists) {
      theme_names.push_back(theme_name);
    }
  }
  return theme_names;
}

std::vector<unsigned char> ColorMap::operator()(double input_value) {
  return evaluate_color(input_value);
}

std::vector<unsigned char> ColorMap::get_edge_color() {
  return edge_colors[theme];
}

void ColorMap::determine_limits() {
  size_t number_of_limits = color_maps[theme].size();
  double step_size = 1. / (number_of_limits - 1);
  limits.clear();
  limits.reserve(number_of_limits);
  for (size_t index = 0; index != number_of_limits; ++index) {
    double limit = step_size * index;
    limits.push_back(limit);
  }
}

std::vector<unsigned char> ColorMap::evaluate_color(double input_value) {
  input_value *= gain;

  if (input_value <= gate) {
    return color_maps[theme].front();
  } else if (input_value >= limits.back()) {
    return color_maps[theme].back();
  } else {
    std::vector<unsigned char> color;
    const auto &color_map = color_maps[theme];
    for (size_t index = 0; index != (limits.size() - 1); ++index) {
      double lower_limit = limits[index];
      double upper_limit = limits[index + 1];
      if (lower_limit <= input_value && input_value < upper_limit) {
        for (size_t rgb_index = 0; rgb_index != 3; ++rgb_index) {
          double lower_value = color_map[index][rgb_index];
          double upper_value = color_map[index + 1][rgb_index];
          std::vector<double> lower_point{lower_limit, lower_value};
          std::vector<double> upper_point{upper_limit, upper_value};
          double value = LinearInterpolation::interpolate(
              lower_point, upper_point, input_value);
          color.push_back(value);
        }
        continue;
      }
    }
    return color;
  }
}

std::vector<std::vector<unsigned char>>
ColorMap::convert_color_map(const std::vector<std::string> &string_color_map) {
  std::vector<std::vector<unsigned char>> color_map;
  color_map.reserve(string_color_map.size());
  for (const auto &string_color : string_color_map) {
    color_map.push_back(RGBColor::hex_string_to_numbers(string_color));
  }
  return color_map;
}
