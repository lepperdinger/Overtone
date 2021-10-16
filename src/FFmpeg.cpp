//
// Created by stefan on 8/14/21.
//

#include "FFmpeg.h"
#include <iostream>

FFmpeg::FFmpeg(std::string input_file_path,
               std::string audio_file_path,
               std::string frames_directory_path,
               std::string video_path,
               std::string ffmpeg_executable_path,
               unsigned frame_rate):
        input_file_path(std::move(input_file_path)),
        audio_file_path(std::move(audio_file_path)),
        frames_directory_path(std::move(frames_directory_path)),
        video_path(std::move(video_path)),
        ffmpeg_executable_path(std::move(ffmpeg_executable_path)),
        frame_rate(frame_rate)
{
}


void FFmpeg::convert_to_wave()
{
    std::string command = "'" + ffmpeg_executable_path
                          + "' -y -i '" + input_file_path
                          + "' '" + audio_file_path + "'";
    std::cout << command << std::endl;
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
    std::string command = "'" + ffmpeg_executable_path
                          + "' -pattern_type glob -framerate "
                          + std::to_string(frame_rate)
                          + " -i '"
                          + add_escape_sequences(frames_directory_path)
                          + "/*.png'"
                          + " -i '" + audio_file_path
                          + "' -b:v 5000k '" + video_path + "'";
    std::cout << command << std::endl;
    int exit_code = std::system(command.c_str());
    if (exit_code)
    {
        throw FFmpeg::file_conversion_error("FFmpeg failed to create the "
                                            "video file '" + video_path
                                            + "'.");
    }
}

std::string FFmpeg::add_escape_sequences(const std::string &input_string)
{
    std::string accumulator;
    for (const char &character : input_string)
    {
        accumulator += std::string("\\") + character;
    }
    return accumulator;
}
