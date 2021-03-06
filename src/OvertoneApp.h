/******************************************************************************

    Overtone: A Music Visualizer

    OvertoneApp.h

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

#ifndef OVERTONE_OVERTONEAPP_H
#define OVERTONE_OVERTONEAPP_H

#include "FFmpeg.h"
#include "Keyboard.h"
#include "Spectrum.h"
#include "WAVE.h"

class OvertoneApp {
public:
  OvertoneApp(int argc, char **argv);
  ~OvertoneApp();
  void run();

private:
  /**
   * Parses the command line arguments.
   */
  void parse_arguments();

  /**
   * Prints the help message.
   */
  void show_help_message() const;

  /**
   * Parses the command line flag pattern -<flag> <value>.
   * @tparam T the type into which the value will be converted
   * @param current_argument iterator to the string that contains the flag
   *                         ("-<flag>"), the following iterator has to be an
   *                         iterator to the string that contains the value
   *                         ("<value>")
   * @param conversion function that converts the value to the type T
   * @param is_positive if true, checks if value > 0
   * @param is_nonzero if true, checks if value != 0
   * @param is_integer if true, checks if the value isn't a floating point
   *                   number
   * @return the parsed and converted value of the flag
   */
  template <typename T>
  T parse_argument(std::vector<std::string>::const_iterator &current_argument,
                   T (*conversion)(const std::string &), bool is_positive,
                   bool is_nonzero, bool is_integer);
  static unsigned to_unsigned(const std::string &s) { return std::stoul(s); };
  static double to_double(const std::string &s) { return std::stod(s); };
  static std::string to_string(const std::string &s) { return s; };

  void evaluate_the_file_paths();
  void create_temporary_directory();
  void create_frames_directory();
  void convert_input_file_to_wav();
  void decode_wav_file();
  void initialize_the_keyboard();
  unsigned evaluate_number_of_video_frames();
  void create_the_video();
  void delete_temporary_files();

  // command line arguments
  std::vector<std::string> arguments;

  // path of the FFmpeg executable
  std::string ffmpeg_executable_path;

  // path of the input file
  std::string input_file_path;

  // path of the audio file that will be created fy FFmpeg
  std::string audio_file_path;

  // directory into which the frames will be saved
  std::string frames_directory_path;

  // path of the final video
  std::string video_path;

  // video frame rate
  unsigned frame_rate;

  double gain;
  double gate;
  std::string theme;

  // speed of the history in lines per video frame
  unsigned history_speed;

  // decoded WAVE file
  WAVE wave;

  FFmpeg ffmpeg;

  // audio spectrum projected onto the 88 keys of the keyboard
  Keyboard keyboard;

  // indices of the audio channels used for the analysis
  std::vector<unsigned> channels;

  // path of the temporary directory
  std::string temporary_directory;
};

#endif // OVERTONE_OVERTONEAPP_H
