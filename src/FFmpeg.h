//
// Created by stefan on 8/14/21.
//

#ifndef OVERTONE_FFMPEG_H
#define OVERTONE_FFMPEG_H


#include <string>
#include <stdexcept>

class FFmpeg
{
public:
    FFmpeg(): frame_rate()
    {
    }

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
    FFmpeg(std::string input_file_path,
           std::string audio_file_path,
           std::string frames_directory_path,
           std::string video_path,
           std::string ffmpeg_executable_path,
           unsigned frame_rate);

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

    std::string get_frames_directory_path() const
    {
        return frames_directory_path;
    }

    std::string get_ffmpeg_executable_path() const
    {
        return ffmpeg_executable_path;
    }
private:
    static std::string add_backslashes(const std::string &input_string);

    std::string input_file_path;
    std::string audio_file_path;
    std::string frames_directory_path;
    std::string video_path;
    std::string ffmpeg_executable_path;
    unsigned frame_rate;
};

class FFmpeg::file_conversion_error: public std::runtime_error
{
public:
    explicit  file_conversion_error(const std::string &message):
        std::runtime_error(message) { }
};

#endif //OVERTONE_FFMPEG_H
