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
  ColorMap ();
  ColorMap (std::string color_map_name, double gain, double gate);
  std::vector<unsigned char> operator() (double input_value);
  std::vector<unsigned char> get_edge_color ();
  std::vector<std::string> get_theme_names ();
  static void run_tests ();

private:
  double gain;
  double gate;
  std::string theme;
  void initialize_themes ();
  bool check_if_theme_exists (const std::string &theme_name);
  std::unordered_map<std::string, std::vector<std::vector<unsigned char> > >
      color_maps;
  std::unordered_map<std::string, std::vector<unsigned char> > edge_colors;
  static double linear_interpolation (std::vector<double> lower_point,
                                      std::vector<double> upper_point,
                                      double x_value);
  std::vector<double> limits;

  static std::vector<unsigned char>
  hex_string_to_rgb_values (const std::string &hex_string);

  static std::vector<std::vector<unsigned char> >
  convert_color_map (const std::vector<std::string> &string_color_map);

  std::vector<unsigned char> evaluate_color (double input_value);

  void determine_limits ();
};

#endif // OVERTONE_COLORMAP_H
