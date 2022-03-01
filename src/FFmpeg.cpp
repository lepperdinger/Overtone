//
// Created by stefan on 8/14/21.
//

#include "FFmpeg.h"
#include <algorithm>
#include <sstream>

FFmpeg::FFmpeg (std::string input_file_path, std::string audio_file_path,
                std::string frames_directory_path, std::string video_path,
                const std::string &ffmpeg_executable_path, unsigned frame_rate)
    : input_file_path (std::move (input_file_path)),
      audio_file_path (std::move (audio_file_path)),
      frames_directory_path (std::move (frames_directory_path)),
      video_path (std::move (video_path)), frame_rate (frame_rate)
{
  std::string command = ffmpeg_executable_path + " -version 1>/dev/null";
  if (std::system (command.c_str ()))
    {
      std::string message;
      if (ffmpeg_executable_path == "ffmpeg")
        {
          message = "It seems that the command 'ffmpeg' does not exist. "
                    "Either install FFmpeg or specify the path of the FFmpeg "
                    "executable via the -F option.";
        }
      else
        {
          std::stringstream message_stream;
          message_stream
              << "The specified file path of the FFmpeg executable, '"
              << ffmpeg_executable_path << "', is invalid.";
          message = message_stream.str ();
        }
      throw std::invalid_argument (message);
    }
  else
    {
      this->ffmpeg_executable_path = ffmpeg_executable_path;
    }
}

void
FFmpeg::convert_to_wave ()
{
  std::string command = "'" + ffmpeg_executable_path + "' -y -i '"
                        + input_file_path + "' '" + audio_file_path
                        + "' 2>/dev/null";
  int exit_code = std::system (command.c_str ());
  if (exit_code)
    {
      throw FFmpeg::file_conversion_error ("FFmpeg failed to convert '"
                                           + input_file_path + "' to '"
                                           + audio_file_path + "'.");
    }
}

void
FFmpeg::convert_to_mp4 ()
{
  std::string command = "'" + ffmpeg_executable_path
                        + "' -pattern_type glob -framerate "
                        + std::to_string (frame_rate) + " -i '"
                        + add_backslashes_if_necessary (frames_directory_path)
                        + "/*.png'" + " -i '" + audio_file_path
                        + "' -b:v 5000k '" + video_path + "' 2>/dev/null";
  int exit_code = std::system (command.c_str ());
  if (exit_code)
    {
      throw FFmpeg::file_conversion_error (
          "FFmpeg failed to create the video file '" + video_path + "'.");
    }
}

std::string
FFmpeg::add_backslashes_if_necessary (const std::string &input_string)
{
  std::string accumulator;
  std::string bad_characters (" \n\t\\[]{}():;\"'<>?|=!$^&*`");
  for (const char &character : input_string)
    {
      auto search_result = std::find (bad_characters.cbegin (),
                                      bad_characters.cend (), character);
      bool bad_character (search_result != bad_characters.cend ());
      if (bad_character)
        {
          accumulator += std::string ("\\");
        }
      accumulator += character;
    }
  return accumulator;
}
