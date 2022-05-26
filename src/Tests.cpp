/******************************************************************************

    Overtone: A Music Visualizer

    Tests.cpp

    Copyright (C) 2022 Stefan Lepperdinger

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

#include "Tests.h"
#include "ColorMap.h"
#include "Keyboard.h"
#include "Spectrum.h"

void Tests::run_all_tests() {
  ColorMap::run_tests();
  Keyboard::run_tests();
  Spectrum::run_tests();
}
