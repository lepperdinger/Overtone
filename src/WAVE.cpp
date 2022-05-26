/******************************************************************************

    Overtone: A Music Visualizer

    WAVE.cpp

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

#include "WAVE.h"
#include <algorithm>
#include <iostream>

void WAVE::read(std::ifstream &file, uint16_t &destination) {
  char bytes[2];
  file.read(bytes, 2);
  destination = static_cast<unsigned char>(bytes[0]) |
                static_cast<unsigned char>(bytes[1]) << 8;
}

void WAVE::read(std::ifstream &file, int16_t &destination) {
  char bytes[2];
  file.read(bytes, 2);
  destination = static_cast<unsigned char>(bytes[0]) |
                static_cast<unsigned char>(bytes[1]) << 8;
}

void WAVE::read(std::ifstream &file, uint32_t &destination) {
  char bytes[4];
  file.read(bytes, 4);
  destination = static_cast<unsigned char>(bytes[0]) |
                static_cast<unsigned char>(bytes[1]) << 8 |
                static_cast<unsigned char>(bytes[2]) << 16 |
                static_cast<unsigned char>(bytes[3]) << 24;
}

void WAVE::read(std::ifstream &file, std::string &destination) {
  char bytes[5];
  file.read(bytes, 4);
  bytes[4] = '\0';
  destination.assign(bytes);
}

void WAVE::search_chunk(std::ifstream &file, const std::string &wanted_chunk_id,
                        uint32_t &found_chunk_size) {
  std::string current_chunk_id;
  while (current_chunk_id != wanted_chunk_id && file) {
    read(file, current_chunk_id);
    read(file, found_chunk_size);
    if (current_chunk_id != wanted_chunk_id) {
      for (uint32_t byte_index = 0; byte_index != found_chunk_size;
           ++byte_index) {
        char tmp;
        file.get(tmp);
      }
    }
  }
  if (!file) {
    throw WAVE::parsing_error("Chunk '" + wanted_chunk_id + "' not found.");
  }
}

void WAVE::parse_riff_chunk_head(std::ifstream &file) {
  read(file, chunk_id);
  if (chunk_id != "RIFF") {
    throw WAVE::parsing_error("File is not a RIFF file.");
  }

  read(file, chunk_size);
  read(file, format);
  if (format != "WAVE") {
    throw WAVE::parsing_error("The RIFF file is not a WAVE file.");
  }
  if (!file) {
    throw WAVE::parsing_error("Parsing of the chunk '" + chunk_id +
                              "' failed.");
  }
}

void WAVE::parse_format_chunk(std::ifstream &file) {
  format_chunk_id = "fmt ";
  search_chunk(file, format_chunk_id, format_chunk_size);

  if (format_chunk_size != 16) {
    throw WAVE::parsing_error("The size of the format chunk isn't 16 bytes.");
  }
  read(file, audio_format);
  if (audio_format != 1) {
    throw WAVE::parsing_error("The audio format isn't PCM.");
  }
  read(file, number_of_channels);
  read(file, sample_rate);
  read(file, byte_rate);
  read(file, block_align);
  read(file, bits_per_sample);
  if (bits_per_sample != 16) {
    throw WAVE::parsing_error("The bit depth isn't 16 bit.");
  }
  if (!file) {
    throw WAVE::parsing_error("Parsing of the chunk '" + format_chunk_id +
                              "' failed.");
  }
}

void WAVE::parse_data_chunk(std::ifstream &file) {
  data_chunk_id = "data";
  search_chunk(file, data_chunk_id, data_chunk_size);
  if (!file) {
    throw WAVE::parsing_error("Parsing of the chunk '" + data_chunk_id +
                              "' failed.");
  }

  using DataSize = std::vector<std::vector<double>>::size_type;
  using ChannelSize = std::vector<double>::size_type;

  ChannelSize number_of_samples_per_channel =
      data_chunk_size / number_of_channels / (bits_per_sample / 8);

  for (DataSize channel = 0; channel != number_of_channels; ++channel) {
    data.emplace_back(
        std::make_shared<std::vector<double>>(std::vector<double>()));
    data.back()->reserve(number_of_samples_per_channel);
  }
  int16_t current_sample;
  for (ChannelSize sample = 0; sample != number_of_samples_per_channel;
       ++sample) {
    for (DataSize channel = 0; channel != number_of_channels; ++channel) {
      read(file, current_sample);
      data[channel]->push_back(current_sample / 32768.0);
    }
  }
  if (!file) {
    throw WAVE::parsing_error("Parsing of the chunk '" + data_chunk_id +
                              "' failed.");
  }
}

void WAVE::decode() {
  std::ifstream file(audio_file_path);
  if (!file) {
    throw std::runtime_error("Couldn't open file: " + audio_file_path);
  }
  parse_riff_chunk_head(file);
  parse_format_chunk(file);
  parse_data_chunk(file);
}
