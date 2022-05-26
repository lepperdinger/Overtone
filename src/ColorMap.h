/******************************************************************************

    Overtone: A Music Visualizer

    ColorMap.h

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

#ifndef OVERTONE_COLORMAP_H
#define OVERTONE_COLORMAP_H
#include <string>
#include <unordered_map>
#include <vector>

class ColorMap {
public:
  /**
   * This constructor exists to be able to get the theme names without being
   * forced to specify a certain theme, e.g.,  ColorMap ().get_theme_names ().
   */
  ColorMap();

  /**
   * @param theme color theme
   * @param gain multiplies each key of the keyboard by this value
   * @param gate all keys below this threshold are set to 0
   */
  ColorMap(std::string theme, double gain, double gate);

  /**
   * Converts a input_value to a RGB color.
   * @param input_value
   * @return color = { red, green, blue }
   */
  std::vector<unsigned char> operator()(double input_value);

  /**
   * @return edge color = { red, green, blue }
   */
  std::vector<unsigned char> get_edge_color();

  /**
   * @return names of all available themes
   */
  std::vector<std::string> get_theme_names();

  static void run_tests();

private:
  // each key of the keyboard is being multiplied by this value
  double gain;

  // all keys below this threshold are set to 0
  double gate;

  std::string theme;

  // corresponding values of the colors of the theme
  // (darkest color = 0., brightest color = 1.)
  std::vector<double> limits;

  std::unordered_map<std::string, std::vector<std::vector<unsigned char>>>
      color_maps;

  std::unordered_map<std::string, std::vector<unsigned char>> edge_colors;

  void initialize_themes();

  bool check_if_theme_exists(const std::string &theme_name);

  static double linear_interpolation(std::vector<double> lower_point,
                                     std::vector<double> upper_point,
                                     double x_value);

  static std::vector<unsigned char>
  hex_string_to_rgb_values(const std::string &hex_string);

  /**
   * @param string_color_map hex strings of the colors, e.g., { "03fa24", ... }
   * @return RGB values of the colors: { { red, green, blue }, ... }
   */
  static std::vector<std::vector<unsigned char>>
  convert_color_map(const std::vector<std::string> &string_color_map);

  /**
   * @param input_value
   * @return RGB values { red, green, blue}
   */
  std::vector<unsigned char> evaluate_color(double input_value);

  void determine_limits();
};

#endif // OVERTONE_COLORMAP_H
