//
// Created by stefan on 2/24/22.
//

#ifndef OVERTONE_COLORMAP_H
#define OVERTONE_COLORMAP_H
#include <string>
#include <unordered_map>
#include <vector>

class ColorMap
{
public:
  /**
   * This constructor exists to be able to get the theme names without being
   * forced to specify a certain theme, e.g.,  ColorMap ().get_theme_names ().
   */
  ColorMap ();

  /**
   * @param theme color theme
   * @param gain multiplies each key of the keyboard by this value
   * @param gate all keys below this threshold are set to 0
   */
  ColorMap (std::string theme, double gain, double gate);

  /**
   * Converts a input_value to a RGB color.
   * @param input_value
   * @return color = { red, green, blue }
   */
  std::vector<unsigned char> operator() (double input_value);

  /**
   * @return edge color = { red, green, blue }
   */
  std::vector<unsigned char> get_edge_color ();

  /**
   * @return names of all available themes
   */
  std::vector<std::string> get_theme_names ();

  static void run_tests ();

private:
  // each key of the keyboard is being multiplied by this value
  double gain;

  // all keys below this threshold are set to 0
  double gate;

  std::string theme;

  std::vector<double> limits;

  void initialize_themes ();
  bool check_if_theme_exists (const std::string &theme_name);
  std::unordered_map<std::string, std::vector<std::vector<unsigned char> > >
      color_maps;
  std::unordered_map<std::string, std::vector<unsigned char> > edge_colors;
  static double linear_interpolation (std::vector<double> lower_point,
                                      std::vector<double> upper_point,
                                      double x_value);

  static std::vector<unsigned char>
  hex_string_to_rgb_values (const std::string &hex_string);

  static std::vector<std::vector<unsigned char> >
  convert_color_map (const std::vector<std::string> &string_color_map);

  std::vector<unsigned char> evaluate_color (double input_value);

  void determine_limits ();
};

#endif // OVERTONE_COLORMAP_H
