//
// Created by stefan on 8/13/21.
//

#include "OvertoneApp.h"
#include "FFmpeg.h"
#include "WAVE.h"
#include "JSON.h"
#include "Spectrum.h"
#include "Keyboard.h"
#include "VideoFrame.h"
#include <string>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <utility>
#include <sys/stat.h>

using std::cout;
using std::cerr;
using std::endl;

OvertoneApp::OvertoneApp(const std::string &input_file_path,
                         const std::string &audio_file_path,
                         const std::string &frames_directory_path,
                         const std::string &video_path,
                         const std::string &ffmpeg_executable_path,
                         unsigned frame_rate,
                         const std::vector<unsigned> &channels,
                         double gain):
    ffmpeg(input_file_path,
           audio_file_path,
           frames_directory_path,
           video_path,
           ffmpeg_executable_path,
           frame_rate),
    wave(audio_file_path)
{
}

void show_color_map(const Keyboard &keyboard, const double &gain)
{
    std::shared_ptr<std::vector<double>> keyboard_ptr(keyboard.get_keyboard());
    for (unsigned key = 0; key != 88; ++ key)
    {
        keyboard_ptr->at(key) = 1 / gain / 87.0 * key;
    }
}

/**
 * Splits the string at the last occurrence of the separator and throws a
 * runtime error if the string doesn't contain the separator.
 * @param string The spring to be split.
 * @param separator The character at which the function splits the string.
 * @return A pair that contains the string before and the string after
 *         the separator.
 */
std::pair<std::string, std::string> split(const std::string &string,
                                          const char &separator)
{
    auto separator_iterator = std::find(string.crbegin(),
                                        string.crend(),
                                        separator);
    if (separator_iterator == string.crend())
    {
        throw std::runtime_error("The string doesn't contain the separator.");
    }
    else
    {
        std::string before_separator(string.cbegin(),
                                     separator_iterator.base() - 1);
        std::string after_separator(separator_iterator.base(),
                                    string.cend());
        return {before_separator, after_separator};
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        cerr << "Usage: Overtone <FFmpeg executable path> <input file path>"
             << endl;
        return 1;
    }

    // Path of the FFmpeg executable.
    std::string ffmpeg_executable_path(argv[1]);
    // Path of the input file.

    std::string input_file_path(argv[2]);

    // Path of the directory that contains the input file.
    std::string input_directory_path;

    std::string input_filename_with_extension;
    try
    {
        input_directory_path = split(input_file_path, '/').first;
        input_filename_with_extension = split(input_file_path, '/').second;
    }
    catch (const std::runtime_error &error)
    {
        // The file path only consists of the filename.
        input_directory_path = '.';
        input_filename_with_extension = input_file_path;
    }

    // Name of the input file (without its file extension).
    std::string input_filename;

    // File extension of the input file.
    std::string file_extension;

    try
    {
        input_filename = split(input_filename_with_extension, '.').first;

        file_extension = split(input_filename_with_extension,
                               '.').second;
    }
    catch (const std::runtime_error &error)
    {
        cerr << "Error: The input file doesn't have a file extension." << endl;
        return 1;
    }

    // Path of the audio file that will be created fy FFmpeg.
    std::string audio_file_path = input_directory_path + '/' + input_filename
                                  + "_-_Overtone.wav";

    std::string frames_directory_path = input_directory_path + '/'
                                        + input_filename
                                        + "_-_Frames_-_Overtone";

    std::string video_path = input_directory_path + '/' + input_filename
                             + "_-_Overtone.mp4";

    if (mkdir(frames_directory_path.c_str(), 0775))
    {
        cerr << "Error: The directory '" << frames_directory_path
             << "' does already exist. Please remove, move, or rename this "
                "directory."
             << endl;
        return 1;
    }
    unsigned frame_rate = 25;
    FFmpeg ffmpeg(input_file_path,
                  audio_file_path,
                  frames_directory_path,
                  video_path,
                  ffmpeg_executable_path,
                  frame_rate);

     //Converting the input file to a WAVE file via FFmpeg
    try
    {
        ffmpeg.convert_to_wave();
    }
    catch(const FFmpeg::file_conversion_error &file_conversion_error)
    {
        std::cerr << file_conversion_error.what() << std::endl;
        std::abort();
    }

    // Decoding the WAVE file
    std::cout << "\nDecoding the WAVE file...\n" << std::endl;
    WAVE wave(audio_file_path);
    wave.show_file_contents();

    std::cout << std::endl;
    std::vector<unsigned> channels = {0};
    Keyboard keyboard = {
            Spectrum(wave, channels, frame_rate, {0, 11}, 67000),
            Spectrum(wave, channels, frame_rate, {11, 22}, 44000),
            Spectrum(wave, channels, frame_rate, {22, 33}, 29000),
            Spectrum(wave, channels, frame_rate, {33, 46}, 15500),
            Spectrum(wave, channels, frame_rate, {46, 56}, 8500),
            Spectrum(wave, channels, frame_rate, {56, 74}, 5000),
            Spectrum(wave, channels, frame_rate, {74, 81}, 2500),
            Spectrum(wave, channels, frame_rate, {81, 88}, 1900)
    };
    double gain = 35.;
    unsigned history_speed = 10;
    VideoFrame video_frame(ffmpeg, gain, history_speed, keyboard);
    unsigned frame = 0;
    while (video_frame.evaluate_frame(frame))
    {
        cout << "Current frame: " << frame << "          \r" << std::flush;
        ++frame;
    }
    cout << endl;

    ffmpeg.convert_to_mp4();
}