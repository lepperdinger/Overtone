//
// Created by stefan on 2/24/22.
//

#include "ColorMap.h"
#include "Tests.h"
#include <vector>


ColorMap::ColorMap(std::string color_map_name, double gain, unsigned char gate):
    gain(gain), gate(gate), color_map_name(std::move(color_map_name))
{
    if (gain < 0)
    {
        throw std::out_of_range("The argument `gain` is negative.");
    }

    color_maps["simple"] = convert_color_map({"101010",
                                              "FFFFFF"});

}

std::vector<std::vector<unsigned char>>
ColorMap::convert_color_map(std::vector<std::string> string_color_map)
{
    std::vector<std::vector<unsigned char>> color_map;
    color_map.reserve(string_color_map.size());
    for (const auto &string_color : string_color_map)
    {
        color_map.push_back(hex_string_to_rgb_values(string_color));
    }
    return color_map;
}

double
ColorMap::linear_interpolation(std::vector<double> lower_point,
                               std::vector<double> upper_point,
                               double x_value)
{
    int x_index = 0;
    int y_index = 1;

    double slope = (lower_point[y_index] - upper_point[y_index]);
    slope /= (lower_point[x_index] - upper_point[x_index]);

    double intersection = upper_point[y_index]*lower_point[x_index];
    intersection -= lower_point[y_index]*upper_point[x_index];
    intersection /= lower_point[x_index] - upper_point[x_index];

    double y_value = intersection + slope*x_value;
    return y_value;
}

std::vector<unsigned char>
ColorMap::hex_string_to_rgb_values(const std::string &hex_string)
{
    std::vector<unsigned char> rgb_values;
    int base{16};

    for (size_t color_index = 0; color_index != 3; ++color_index)
    {
        size_t string_index = color_index * 2;
        std::string sub_string = hex_string.substr(string_index, 2);
        unsigned char rgb_value = std::strtoul(sub_string.c_str(),
                                               nullptr,
                                               base);
        rgb_values.push_back(rgb_value);
    }

    return rgb_values;
}

void
ColorMap::run_tests()
{
    {
        double tolerance = 1E-10;
        double expected = 1199.452154999999948;
        double result = linear_interpolation({32.4, 857.816654999999969},
                                             {132.43, 5183.614004999999452},
                                             40.3);
        TEST(std::abs(expected - result) < tolerance)
    }
    {
        std::vector<unsigned char> expected{0xff, 0xff, 0xff};
        TEST(hex_string_to_rgb_values("ffffff") == expected)
        TEST(hex_string_to_rgb_values("FFFFFF") == expected)
        TEST(hex_string_to_rgb_values("FfFffF") == expected)

        expected = {0x0, 0x0, 0x0};
        TEST(hex_string_to_rgb_values("000000") == expected)

        expected = {0xab, 0x51, 0x22};
        TEST(hex_string_to_rgb_values("ab5122") == expected)
    }
}
