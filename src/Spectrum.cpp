/******************************************************************************

    Overtone: A Music Visualizer

    Spectrum.cpp

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

#include "Spectrum.h"
#include "KeyboardFrequencies.h"
#include <cmath>

Spectrum::Spectrum(const WAVE &wave, const std::vector<unsigned> &channels,
                   const unsigned &frame_rate, KeyRange key_range,
                   const VectorSize &minimum_samples)
    : wave(wave), samples_per_video_frame(wave.get_sample_rate() / frame_rate),
      time_range_video_frame(0, samples_per_video_frame),
      key_range(std::move(key_range)), minimum_samples(minimum_samples),
      time_size(wave.get_signal()[0]->size()),
      frequencies(std::make_shared<Vector>()) {
  if (key_range.first > 87 || key_range.second > 88 ||
      key_range.second <= key_range.first) {
    throw std::invalid_argument(
        "0 <= key_range.first < key_range.second <= 88 not fulfilled.");
  }
  if (wave.get_sample_rate() % frame_rate) {
    throw std::invalid_argument(
        "This frame rate is not available. (sample rate % frame rate != 0)");
  }
  VectorSize number_of_channels = wave.get_signal().size();
  for (auto channel : channels) {
    if (channel >= number_of_channels) {
      throw std::invalid_argument("Channel " + std::to_string(channel) +
                                  " does not exist.");
    }
  }
  std::vector<unsigned> unique_channels = channels;
  auto unique_end = std::unique(unique_channels.begin(), unique_channels.end());
  if (unique_end != unique_channels.cend()) {
    throw std::invalid_argument("The elements of channels are not unique.");
  }
  if (channels.empty()) {
    for (unsigned channel = 0; channel != number_of_channels; ++channel) {
      this->channels.push_back(channel);
    }
  } else {
    this->channels = channels;
  }
  evaluate_frame();
}

bool Spectrum::go_to_next_frame() {
  time_range_video_frame.first += samples_per_video_frame;
  time_range_video_frame.second += samples_per_video_frame;
  if (time_range_video_frame.second <= time_size) {
    evaluate_frame();
    return true;
  } else {
    return false;
  }
}

void Spectrum::evaluate_frame() {
  VectorRange time_range = evaluate_time_range();
  Vector all_frequencies =
      evaluate_all_frequencies(time_range, wave.get_sample_rate());
  VectorRange frequency_range =
      KeyboardFrequencies::key_range_to_frequency_range(key_range,
                                                        all_frequencies);
  auto all_frequencies_begin = all_frequencies.cbegin();
  frequencies->assign(all_frequencies_begin + frequency_range.first,
                      all_frequencies_begin + frequency_range.second);
  keys = std::make_shared<Vector>(
      KeyboardFrequencies::frequencies_to_keys(*frequencies));
  spectrum = std::make_shared<Vector>(evaluate_spectrum(
      wave.get_signal(), channels, time_range, frequency_range));
}

Spectrum::VectorRange Spectrum::evaluate_time_range() {
  if (samples_per_video_frame >= minimum_samples) {
    return time_range_video_frame;
  } else {
    VectorRange time_range;
    VectorSize half_missing_samples =
        ((minimum_samples - samples_per_video_frame) / 2);
    if (time_range_video_frame.first > half_missing_samples) {
      time_range.first = time_range_video_frame.first - half_missing_samples;
    } else {
      time_range.first = 0;
    }
    VectorSize new_index = time_range_video_frame.second + half_missing_samples;
    if (new_index < time_size) {
      time_range.second = new_index;
    } else {
      time_range.second = time_size;
    }
    return time_range;
  }
}

Spectrum::Vector
Spectrum::evaluate_spectrum(const std::vector<std::shared_ptr<Vector>> &signal,
                            const std::vector<unsigned> &channels,
                            const VectorRange &time_range,
                            const VectorRange &frequency_range) {
  auto number_of_channels = channels.size();

  std::vector<Vector> spectra;
  spectra.reserve(number_of_channels);
  for (const unsigned &channel : channels) {
    spectra.emplace_back(evaluate_channel_spectrum(*signal[channel], time_range,
                                                   frequency_range));
  }

  Vector returned_spectrum;
  auto spectrum_size = spectra[0].size();
  returned_spectrum.reserve(spectrum_size);

  // Evaluating the average spectrum.
  double accumulator;
  for (VectorSize index = 0; index != spectrum_size; ++index) {
    accumulator = 0;
    for (const Vector &current_spectrum : spectra) {
      accumulator += current_spectrum[index];
    }
    returned_spectrum.push_back(accumulator / number_of_channels);
  }
  return returned_spectrum;
}

Spectrum::Vector
Spectrum::evaluate_channel_spectrum(const Vector &channel,
                                    const VectorRange &time_range,
                                    const VectorRange &frequency_range) {
  Vector spectrum;
  VectorSize number_of_samples = time_range.second - time_range.first;
  spectrum.reserve(frequency_range.second - frequency_range.first);
  double fourier_negative_imaginary_part;
  double fourier_real_part;
  double constant = 2 * M_PI / number_of_samples;
  double current_sample;
  double frequency_index_double;
  for (VectorSize frequency_index = frequency_range.first;
       frequency_index != frequency_range.second; ++frequency_index) {
    fourier_negative_imaginary_part = 0;
    fourier_real_part = 0;
    frequency_index_double = frequency_index;
    for (VectorSize time_index = time_range.first;
         time_index != time_range.second; ++time_index) {
      current_sample = channel[time_index];
      fourier_negative_imaginary_part +=
          (current_sample *
           sin(constant * frequency_index_double * time_index));
      fourier_real_part +=
          (current_sample *
           cos(constant * frequency_index_double * time_index));
    }
    spectrum.push_back(2. *
                       abs(fourier_real_part, fourier_negative_imaginary_part) /
                       number_of_samples);
  }
  return spectrum;
}

Spectrum::Vector
Spectrum::evaluate_all_frequencies(const VectorRange &time_range,
                                   const VectorSize &sample_rate) {
  Vector frequencies;
  VectorSize number_of_samples = time_range.second - time_range.first;

  frequencies.reserve(number_of_samples / 2);
  double frequency_step_size = 1. * sample_rate / number_of_samples;
  for (VectorSize frequency_index = 0; frequency_index != number_of_samples / 2;
       ++frequency_index) {
    frequencies.push_back(frequency_index * frequency_step_size);
  }
  return frequencies;
}

double Spectrum::sqr(double value) { return value * value; }

double Spectrum::abs(double real_part, double imaginary_part) {
  return sqrt(sqr(real_part) + sqr(imaginary_part));
}
