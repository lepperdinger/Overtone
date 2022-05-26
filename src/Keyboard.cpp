/******************************************************************************

    Overtone: A Music Visualizer

    Keyboard.cpp

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

#include "Keyboard.h"

Keyboard::Keyboard(std::initializer_list<Spectrum> spectra)
    : spectra(spectra), keyboard(std::make_shared<Vector>()) {
  evaluate_keys();
}

void Keyboard::evaluate_keys() {
  keyboard->assign(88, 0.0);
  KeyRange key_range;
  double weighted_spectrum_accumulator, weight_accumulator, weight;
  Vector::const_iterator key, keys_end, spectrum_value;
  for (const Spectrum &spectrum : spectra) {
    key = spectrum.get_keys()->cbegin();
    keys_end = spectrum.get_keys()->cend();
    spectrum_value = spectrum.get_spectrum()->cbegin();
    key_range = spectrum.get_key_range();
    for (unsigned char assigned_key = key_range.first;
         assigned_key != key_range.second; ++assigned_key) {
      weighted_spectrum_accumulator = 0.0;
      weight_accumulator = 0.0;
      while (key != keys_end && assigned_key - 0.5 <= *key &&
             *key < assigned_key + 0.5) {
        weight = evaluate_weight(*key, assigned_key);
        weighted_spectrum_accumulator += *spectrum_value * weight;
        weight_accumulator += weight;
        ++key;
        ++spectrum_value;
      }
      if (weighted_spectrum_accumulator != 0.0) {
        (*keyboard)[assigned_key] +=
            weighted_spectrum_accumulator / weight_accumulator;
      }
    }
  }
}

double Keyboard::evaluate_weight(const double &key,
                                 const unsigned char &assigned_key) {
  // The upwards shift by 0.01 is necessary to avoid divisions by zero.
  if (key < assigned_key) {
    return 2. * key + 1. - 2. * assigned_key + 0.01;
  } else {
    return -2. * key + 1. + 2. * assigned_key + 0.01;
  }
}

bool Keyboard::go_to_next_frame() {
  bool valid = false;
  for (Spectrum &spectrum : spectra) {
    valid = spectrum.go_to_next_frame();
  }
  if (valid) {
    evaluate_keys();
  }
  return valid;
}
