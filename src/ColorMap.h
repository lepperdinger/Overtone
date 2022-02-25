//
// Created by stefan on 2/24/22.
//

#ifndef OVERTONE_COLORMAP_H
#define OVERTONE_COLORMAP_H
#include <string>
#include <vector>
#include <map>


class ColorMap {
public:
    ColorMap(std::string color_map_name, double gain, unsigned char gate);
    static void run_tests();
private:
    double gain;
    unsigned char gate;
    std::string color_map_name;
    std::map<std::string, std::vector<std::vector<unsigned char>>> color_maps;
    std::vector<std::vector<unsigned char>> color_map;
    static double linear_interpolation(std::vector<double> lower_point,
                                       std::vector<double> upper_point,
                                       double x_value);

    static std::vector<unsigned char> hex_string_to_rgb_values(
        const std::string &hex_string
    );

    static std::vector<std::vector<unsigned char>> convert_color_map(
            std::vector<std::string> string_color_map
    );
};


#endif //OVERTONE_COLORMAP_H
