/******************************************************************************

    Overtone: A Music Visualizer

    FFmpeg.h

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

#ifndef OVERTONE_FFMPEG_H
#define OVERTONE_FFMPEG_H

#include <stdexcept>
#include <string>

class FFmpeg {
public:
  FFmpeg() : frame_rate() {}

  /**
   * Configures the file paths for the FFmpeg commands.
   * @param input_file_path Video or audio file.
   * @param audio_file_path WAVE file for the video
   *                        (signed 16 bit linear-PCM).
   * @param frames_directory_path Directory for the frames of the video.
   * @param video_path Final video
   * @param ffmpeg_executable_path Path of the FFmpeg executable
   * @param frame_rate Video frame rate in frames per second
   */
  FFmpeg(std::string input_file_path, std::string audio_file_path,
         std::string frames_directory_path, std::string video_path,
         const std::string &ffmpeg_executable_path, unsigned frame_rate);

  /**
   * An exception that occurs if the FFmpeg returns an exit code that is not
   * 0.
   */
  class file_conversion_error;

  /**
   * Converts the file `input_file_path` to a WAVE file that contains a 16 bit
   * linear-PCM signal (signed and little endian).
   */
  void convert_to_wave();

  /**
   * Converts the frames in `frames_directory_path` and the audio file
   * `audio_file_path` to a video and saves it into the file `video_path`
   */
  void convert_to_mp4();

  std::string get_frames_directory_path() const {
    return frames_directory_path;
  }

  std::string get_ffmpeg_executable_path() const {
    return ffmpeg_executable_path;
  }

private:
  static std::string
  add_backslashes_if_necessary(const std::string &input_string);

  std::string input_file_path;
  std::string audio_file_path;
  std::string frames_directory_path;
  std::string video_path;
  std::string ffmpeg_executable_path;
  unsigned frame_rate;
};

class FFmpeg::file_conversion_error : public std::runtime_error {
public:
  explicit file_conversion_error(const std::string &message)
      : std::runtime_error(message) {}
};

#endif // OVERTONE_FFMPEG_H
