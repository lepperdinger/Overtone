/******************************************************************************

    Overtone: A Music Visualizer

    Spectrum.h

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

#ifndef OVERTONE_SPECTRUM_H
#define OVERTONE_SPECTRUM_H

#include "WAVE.h"
#include <algorithm>
#include <cmath>
#include <limits>
/**
 * Class for evaluating the audio spectra for each video frame
 */
class Spectrum
{
public:
  using Vector = std::vector<double>;
  using VectorSize = Vector::size_type;
  using VectorRange = std::pair<VectorSize, VectorSize>;
  using KeyRange = std::pair<unsigned char, unsigned char>;

  /**
   * Evaluates the spectrum of the first frame. The spectrum is the average
   * of the spectra of the selected channels. The spectrum gets evaluated
   * for frequencies between the key range key_range.first - 0.5 and
   * key_range.second + 0.5. If the number of audio samples within a video
   * frame is smaller than minimum_samples, the range of an audio frame gets
   * extended on both sides.
   * @param wave WAVE object that contains the PCM signal.
   * @param channels selected channels (all channels if empty)
   * @param frame_rate video frame rate
   * @param key_range key range
   * @param minimum_samples minimum audio samples per video frame
   */
  explicit Spectrum (const WAVE &wave, const std::vector<unsigned> &channels,
                     const unsigned &frame_rate, KeyRange key_range,
                     const VectorSize &minimum_samples);

  /**
   * Evaluates the next frame.
   * @return False if there is no next video frame and an evaluation of the
   *         next frame, therefore, isn't possible.
   */
  bool go_to_next_frame ();

  /**
   * Returns the spectrum.
   * @return spectrum
   */
  std::shared_ptr<Vector>
  get_spectrum () const
  {
    return spectrum;
  }

  /**
   * Returns the keyboard of the spectrum.
   * @return keys
   */
  std::shared_ptr<Vector>
  get_keys () const
  {
    return keys;
  }

  /**
   * Returns the key range within which the spectrum has been evaluated.
   * @return key range
   */
  KeyRange
  get_key_range () const
  {
    return key_range;
  }

  static void run_tests ();

private:
  // WAVE object that contains the PCM signal.
  WAVE wave;

  // the number of audio samples per video frame
  const VectorSize samples_per_video_frame;

  // the time index range of a video frame
  VectorRange time_range_video_frame;

  // the key range within which the spectrum gets evaluated
  KeyRange key_range;

  // The minimum number of audio samples.
  VectorSize minimum_samples;

  // the number of audio samples in the PCM signal
  VectorSize time_size;

  // the selected channels for which the spectrum gets evaluated
  std::vector<unsigned> channels;

  // the frequencies of the current spectrum
  std::shared_ptr<Vector> frequencies;

  // the keyboard of the current spectrum
  std::shared_ptr<Vector> keys;

  // the spectrum of the current video frame
  std::shared_ptr<Vector> spectrum;

  /**
   * Evaluates the spectrum of the current video frame.
   */
  void evaluate_frame ();

  /**
   * Evaluates the time index range of the current video frame. The time range
   * gets extended if samples_per_video_frame >= minimum_samples.
   * @return time index range
   */
  VectorRange evaluate_time_range ();

  /**
   * Evaluates the spectrum of a single channel within a specified time and
   * frequency range.
   * @param channel PCM signal of the channel.
   * @param time_range time index range
   * @param frequency_range frequency range
   * @return spectrum of the selected channel
   */
  static Vector evaluate_channel_spectrum (const Vector &channel,
                                           const VectorRange &time_range,
                                           const VectorRange &frequency_range);
  static Spectrum::Vector
  evaluate_spectrum (const std::vector<std::shared_ptr<Vector> > &signal,
                     const std::vector<unsigned> &channels,
                     const VectorRange &time_range,
                     const VectorRange &frequency_range);

  /**
   * Evaluates all the frequencies of the Fourier transform. The Fourier
   * transform gets then evaluated only for the frequencies within the
   * selected key range.
   * @param time_range time index range of the current frame
   * @param sample_rate audio sample rate
   * @return all frequencies of the Fourier transform
   */
  static Vector evaluate_all_frequencies (const VectorRange &time_range,
                                          const VectorSize &sample_rate);

  /**
   * Returns the index range of the frequencies between the key range
   * key_range.first - 0.5 and key_range.second + 0.5
   * @param key_range selected key range
   * @param all_frequencies all frequencies of the Fourier transform
   * @return frequencies that contain the specified key range
   */
  static VectorRange
  key_range_to_frequency_range (const KeyRange &key_range,
                                const Vector &all_frequencies);

  /**
   * Converts the frequencies to keyboard.
   * @param frequencies frequencies
   * @return keys
   */
  inline static Vector evaluate_keys (const Vector &frequencies);

  /**
   * Square root.
   * @param value input value
   * @return square root of value
   */
  inline static double sqr (double value);

  /**
   * Absolute value of a complex number.
   * @param real_part real part of the complex number
   * @param imaginary_part imaginary part of the complex number
   * @return absolute value of the complex number
   */
  inline static double abs (double real_part, double imaginary_part);

  /**
   * Converts a key to a frequency.
   * @param key key
   * @return frequency
   */
  inline static double keys_to_frequencies (const double &key);

  /**
   * Converts a frequency to a key.
   * @param frequency frequency
   * @return key
   */
  inline static double frequencies_to_keys (const double &frequency);
};

#endif // OVERTONE_SPECTRUM_H
