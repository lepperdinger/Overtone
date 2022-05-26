/******************************************************************************

    Overtone: A Music Visualizer

    LinearInterpolation.cpp

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

#include "LinearInterpolation.h"

double LinearInterpolation::interpolate(std::vector<double> lower_point,
                                        std::vector<double> upper_point,
                                        double x_value) {
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
