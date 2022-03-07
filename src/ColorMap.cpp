//
// Created by stefan on 2/24/22.
//

#include "ColorMap.h"
#include "Tests.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

ColorMap::ColorMap (std::string theme, double gain, double gate)
    : gain (gain), gate (gate), theme (std::move (theme))
{
  if (gain < 0)
    {
      throw std::out_of_range ("The argument `gain` is negative.");
    }

  if (gate < 0 || gate > 1)
    {
      throw std::out_of_range (
          "The argument `gate` is not within the interval 0 <= gate <= 1.");
    }

  edge_colors["gray"] = hex_string_to_rgb_values ("000000");
  color_maps["gray"] = convert_color_map ({ "202020", "ffffff" });

  edge_colors["white"] = hex_string_to_rgb_values ("202020");
  color_maps["white"] = convert_color_map ({ "ffffff", "000000" });

  edge_colors["matrix"] = hex_string_to_rgb_values ("003000");
  color_maps["matrix"] = convert_color_map ({ "000000", "00ff00" });

  edge_colors["cyan"] = hex_string_to_rgb_values ("000000");
  color_maps["cyan"]
      = convert_color_map ({ "202020", "349d8c", "7ce1d2", "dcdadb" });

  edge_colors["fire"] = hex_string_to_rgb_values ("000000");
  color_maps["fire"] = convert_color_map (
      { "20030d", "32021b", "6f0511", "980506", "cb0503", "df3405", "eb6001",
        "f78c01", "f6a805", "febb08" });

  edge_colors["vintage"] = hex_string_to_rgb_values ("000000");
  color_maps["vintage"]
      = convert_color_map ({ "231f22", "777b62", "999a7c", "dbaa93", "efdfa4",
                             "facd66", "d08e5f", "9b7e68" });

  edge_colors["purple"] = hex_string_to_rgb_values ("000000");
  color_maps["purple"]
      = convert_color_map ({ "0e042c", "230044", "3c076c", "581d96", "7d2cbc",
                             "a050df", "c57ffa", "e0adfb" });

  bool color_map_exists = color_maps.find (this->theme) != color_maps.end ();
  bool edge_color_exists
      = edge_colors.find (this->theme) != edge_colors.end ();
  if (!(color_map_exists && edge_color_exists))
    {
      auto theme_names = get_theme_names ();
      std::stringstream message;
      message << "Overtone: Error: Theme '" << this->theme
              << "' not found. Available themes:\n";
      for (auto theme_name = theme_names.cbegin ();
           theme_name != theme_names.cend (); ++theme_name)
        {
          message << "    -> " << *theme_name;
          if (theme_name != theme_names.cend () - 1)
            {
              message << '\n';
            }
        }
      throw std::invalid_argument (message.str ());
    }
  determine_limits ();
}

std::vector<std::string>
ColorMap::get_theme_names ()
{
  std::vector<std::string> theme_names;
  theme_names.reserve (color_maps.size ());
  for (const auto &color_map : color_maps)
    {
      std::string theme_name (color_map.first);
      bool edge_color_exists
          = edge_colors.find (this->theme) != edge_colors.end ();
      if (!edge_color_exists)
        {
          theme_names.push_back (theme_name);
        }
    }
  return theme_names;
}

std::vector<unsigned char>
ColorMap::operator() (double input_value)
{
  return evaluate_color (input_value);
}

std::vector<unsigned char>
ColorMap::get_edge_color ()
{
  return edge_colors[theme];
}

void
ColorMap::determine_limits ()
{
  size_t number_of_limits = color_maps[theme].size ();
  double step_size = 1. / (number_of_limits - 1);
  limits.clear ();
  limits.reserve (number_of_limits);
  for (size_t index = 0; index != number_of_limits; ++index)
    {
      double limit = step_size * index;
      limits.push_back (limit);
    }
}

std::vector<unsigned char>
ColorMap::evaluate_color (double input_value)
{
  input_value *= gain;

  if (input_value <= gate)
    {
      return color_maps[theme].front ();
    }
  else if (input_value >= limits.back ())
    {
      return color_maps[theme].back ();
    }
  else
    {
      std::vector<unsigned char> color;
      const auto &color_map = color_maps[theme];
      for (size_t index = 0; index != (limits.size () - 1); ++index)
        {
          double lower_limit = limits[index];
          double upper_limit = limits[index + 1];
          if (lower_limit <= input_value && input_value < upper_limit)
            {
              for (size_t rgb_index = 0; rgb_index != 3; ++rgb_index)
                {
                  double lower_value = color_map[index][rgb_index];
                  double upper_value = color_map[index + 1][rgb_index];
                  std::vector<double> lower_point{ lower_limit, lower_value };
                  std::vector<double> upper_point{ upper_limit, upper_value };
                  double value = linear_interpolation (
                      lower_point, upper_point, input_value);
                  color.push_back (value);
                }
              continue;
            }
        }
      return color;
    }
}

std::vector<std::vector<unsigned char> >
ColorMap::convert_color_map (const std::vector<std::string> &string_color_map)
{
  std::vector<std::vector<unsigned char> > color_map;
  color_map.reserve (string_color_map.size ());
  for (const auto &string_color : string_color_map)
    {
      color_map.push_back (hex_string_to_rgb_values (string_color));
    }
  return color_map;
}

double
ColorMap::linear_interpolation (std::vector<double> lower_point,
                                std::vector<double> upper_point,
                                double x_value)
{
  int x_index = 0;
  int y_index = 1;

  double slope = (lower_point[y_index] - upper_point[y_index]);
  slope /= (lower_point[x_index] - upper_point[x_index]);

  double intersection = upper_point[y_index] * lower_point[x_index];
  intersection -= lower_point[y_index] * upper_point[x_index];
  intersection /= lower_point[x_index] - upper_point[x_index];

  double y_value = intersection + slope * x_value;
  return y_value;
}

std::vector<unsigned char>
ColorMap::hex_string_to_rgb_values (const std::string &hex_string)
{
  std::vector<unsigned char> rgb_values;
  int base{ 16 };

  for (size_t color_index = 0; color_index != 3; ++color_index)
    {
      size_t string_index = color_index * 2;
      std::string sub_string = hex_string.substr (string_index, 2);
      unsigned char rgb_value
          = std::strtoul (sub_string.c_str (), nullptr, base);
      rgb_values.push_back (rgb_value);
    }

  return rgb_values;
}

void
ColorMap::run_tests ()
{
  {
    double tolerance = 1E-10;
    double expected = 1199.452154999999948;
    double result = linear_interpolation (
        { 32.4, 857.816654999999969 }, { 132.43, 5183.614004999999452 }, 40.3);
    TEST (std::abs (expected - result) < tolerance)
  }
  {
    std::vector<unsigned char> expected{ 0xff, 0xff, 0xff };
    TEST (hex_string_to_rgb_values ("ffffff") == expected)
    TEST (hex_string_to_rgb_values ("FFFFFF") == expected)
    TEST (hex_string_to_rgb_values ("FfFffF") == expected)

    expected = { 0x0, 0x0, 0x0 };
    TEST (hex_string_to_rgb_values ("000000") == expected)

    expected = { 0xab, 0x51, 0x22 };
    TEST (hex_string_to_rgb_values ("ab5122") == expected)
  }
}
