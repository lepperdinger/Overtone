/******************************************************************************

    Overtone: A Music Visualizer

    Keyboard.h

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

#ifndef OVERTONE_KEYBOARD_H
#define OVERTONE_KEYBOARD_H

#include "Spectrum.h"
#include <initializer_list>
#include <memory>

/**
 * This class projects the audio spectra onto the 88 keys of the keyboard.
 */
class Keyboard
{
public:
  using Vector = std::vector<double>;
  using KeyRange = std::pair<unsigned char, unsigned char>;

  Keyboard () = default;

  /**
   * constructor that evaluates `keyboard` for the first frame of the video
   * @param spectra audio spectra of the keyboard sections
   */
  Keyboard (std::initializer_list<Spectrum> spectra);

  std::shared_ptr<Vector>
  get_keyboard () const
  {
    return keyboard;
  }

  /**
   * Evaluates `keyboard` for the next video frame if there is a next video
   * frame.
   * @return false if there is no next video frame
   */
  bool go_to_next_frame ();

  static void run_tests ();

private:
  // audio spectra of the keyboard sections
  std::vector<Spectrum> spectra;

  // vector that contains the signal of each key of the keyboard
  std::shared_ptr<Vector> keyboard;

  /**
   * Evaluates `keyboard`.
   */
  void evaluate_keys ();

  /**
   * evaluate_keys() evaluates the weighted average of all the signals that
   * belong to a certain key on the keyboard. This function determines the
   * weight of such a signal.
   * @param key position of a certain frequency on the keyboard, e.g., 440 Hz
   *            would be at 48.0
   * @param assigned_key index of the key on the keyboard to which the signal
   *                     belongs, i.e., an integer from 0 to 87
   * @return weight
   */
  inline static double evaluate_weight (const double &key,
                                        const unsigned char &assigned_key);
};

#endif // OVERTONE_KEYBOARD_H
