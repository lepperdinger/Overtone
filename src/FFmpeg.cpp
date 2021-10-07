//
// Created by stefan on 8/14/21.
//

#include "FFmpeg.h"
#include <iostream>


void FFmpeg::convert_to_wave()
{
    std::string command = ffmpeg_executable_path
                          + " -y -i " + input_file_path
                          + " " + audio_file_path;
    int exit_code = std::system(command.c_str());
    if (exit_code)
    {
        throw FFmpeg::file_conversion_error("FFmpeg failed to convert '"
                                            + input_file_path
                                            + "' to '" + audio_file_path
                                            + "'.");
    }
}


void FFmpeg::convert_to_mp4()
{
    std::string command = ffmpeg_executable_path
                          + " -pattern_type glob -framerate "
                          + std::to_string(frame_rate)
                          + " -i '" + frames_directory_path + "/*.png'"
                          + " -i " + audio_file_path
                          + " -b:v 5000k " + video_path;
    int exit_code = std::system(command.c_str());
    if (exit_code)
    {
        throw FFmpeg::file_conversion_error("FFmpeg failed to create the "
                                            "video file '" + video_path
                                            + "'.");
    }
}