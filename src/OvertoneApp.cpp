//
// Created by stefan on 8/13/21.
//

#include "OvertoneApp.h"
#include "FFmpeg.h"
#include "WAVE.h"
#include "Spectrum.h"
#include "Keyboard.h"
#include "VideoFrame.h"
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <sys/stat.h>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;

OvertoneApp::OvertoneApp(std::vector<std::string> arguments):
    arguments(std::move(arguments))
{
    frame_rate = 25;
    gain = 35.;
    history_speed = 10;
    ffmpeg_executable_path = "ffmpeg";
}

std::pair<std::string, std::string> OvertoneApp::split(
    const std::string &string,
    const char &separator
)
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

void OvertoneApp::parse_arguments()
{
    std::string usage = "Overtone [options]... <input file path>";
    std::stringstream descriptions_stream;
    descriptions_stream
        << "Optional arguments:\n"
           "  -c  <channel>                 use a specific audio channel "
                                           "instead of all\n"
           "                                channels (e.g., 0)\n"
           "  -f  <frame rate>              frame rate in frames per seconds "
                                           "(default = " << frame_rate << ")\n"
           "  -F  <ffmpeg executable path>  path of the FFmpeg executable\n"
           "  -g  <gain>                    gain (default = " << gain << ")\n"
           "  -s  <history speed>           speed of the history in lines per "
                                           "frame (default = " << history_speed
                                            << ")";
    std::string descriptions = descriptions_stream.str();
    if (arguments.size() == 1)
    {
        cout << usage << endl << endl;
        cout << descriptions << endl << endl;
        std::exit(1);
    }
    std::vector<std::string> positional_arguments;
    for (auto argument = ++arguments.cbegin(); argument != arguments.cend();
         ++argument)
    {
        if (*argument == "-c")
        {
            unsigned channel = parse_argument(argument,
                                              &OvertoneApp::to_unsigned);
            channels = {channel};
        }
        else if (*argument == "-f")
        {
            frame_rate = parse_argument(argument, &OvertoneApp::to_unsigned);
        }
        else if (*argument == "-F")
        {
            ffmpeg_executable_path = parse_argument(argument,
                                                    &OvertoneApp::to_string);
        }
        else if (*argument == "-g")
        {
            gain = parse_argument(argument, &OvertoneApp::to_double);
            if (gain <= 0)
            {
                cerr << "Error: The gain has to be a positive number." << endl;
                exit(1);
            }
        }
        else if (*argument == "-s")
        {
            history_speed = parse_argument(argument, &OvertoneApp::to_unsigned);
        }
        else if ((*argument)[0] == '-')
        {
            std::string error_message = "Error: unrecognized argument: "
                                        + *argument;
            cerr << error_message << endl;
            exit(1);
        }
        else
        {
            positional_arguments.push_back(*argument);
        }
    }
    unsigned number_of_positional_arguments = 1;
    if (positional_arguments.size() != number_of_positional_arguments)
    {
        cout << "Error: the following arguments are required: "
             << "<input file path>" << endl;
        std::exit(1);
    }
    else
    {
        input_file_path.assign(positional_arguments[0]);
    }
}

template <typename T>
T OvertoneApp::parse_argument(
    std::vector<std::string>::const_iterator &current_argument,
    T (*conversion)(const std::string &)
)
{
    T parsed_value;
    std::string flag = *current_argument++;
    std::string error_message = "Error: argument "
                                + flag
                                + ": invalid value";
    if (current_argument == arguments.cend())
    {
        cout << error_message << endl;
        std::exit(1);
    }
    else
    {
        try
        {
            parsed_value = (*conversion)(*current_argument);
        }
        catch (const std::invalid_argument &invalid_argument)
        {
            cout << error_message << endl;
            std::exit(1);
        }
    }
    return parsed_value;
}

void OvertoneApp::evaluate_the_file_paths()
{
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
    try
    {
        input_filename = split(input_filename_with_extension, '.').first;

        file_extension = split(input_filename_with_extension,
                               '.').second;
    }
    catch (const std::runtime_error &error)
    {
        cerr << "Error: The input file doesn't have a file extension." << endl;
        exit(1);
    }

    // Path of the audio file that will be created fy FFmpeg.
    audio_file_path = input_directory_path + '/' + input_filename
                      + "_-_Overtone.wav";

    frames_directory_path = input_directory_path + '/'
                            + input_filename
                            + "_-_Frames_-_Overtone";

    video_path = input_directory_path + '/' + input_filename
                 + "_-_Overtone.mp4";
}

void OvertoneApp::create_frames_directory()
{
    if (mkdir(frames_directory_path.c_str(), 0775))
    {
        cerr << "Error: The directory '" << frames_directory_path
             << "' does already exist. Please remove, move, or rename this "
                "directory."
             << endl;
        exit(1);
    }
}

void OvertoneApp::convert_input_file_to_wav()
{
    ffmpeg = FFmpeg(input_file_path,
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
        cout << file_conversion_error.what() << endl;
        std::abort();
    }
}

void OvertoneApp::decode_wav_file()
{
    cout << "\nDecoding the WAVE file...\n" << endl;
    wave = WAVE(audio_file_path);
    wave.show_file_contents();
}

void OvertoneApp::initialize_the_keyboard() {
    cout << endl;
    keyboard = {
            Spectrum(wave, channels, frame_rate, {0, 11}, 67000),
            Spectrum(wave, channels, frame_rate, {11, 22}, 44000),
            Spectrum(wave, channels, frame_rate, {22, 33}, 29000),
            Spectrum(wave, channels, frame_rate, {33, 46}, 15500),
            Spectrum(wave, channels, frame_rate, {46, 56}, 8500),
            Spectrum(wave, channels, frame_rate, {56, 74}, 5000),
            Spectrum(wave, channels, frame_rate, {74, 81}, 2500),
            Spectrum(wave, channels, frame_rate, {81, 88}, 1900)
    };
}

void OvertoneApp::create_the_video()
{
    VideoFrame video_frame(ffmpeg, gain, history_speed, keyboard);
    unsigned frame = 0;
    while (video_frame.evaluate_frame(frame))
    {
        cout << "Current frame: " << frame << "          \r" << std::flush;
        ++frame;
    }
    cout << endl;

    ffmpeg.convert_to_mp4();}

void OvertoneApp::run()
{
    parse_arguments();
    evaluate_the_file_paths();
    create_frames_directory();
    convert_input_file_to_wav();
    decode_wav_file();
    initialize_the_keyboard();
    create_the_video();
}

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments;
    for (int index = 0; index != argc; ++index)
    {
        arguments.emplace_back(argv[index]);
    }
    OvertoneApp overtone_app(arguments);
    overtone_app.run();
}

void show_color_map(const Keyboard &keyboard, const double &gain)
{
    std::shared_ptr<std::vector<double>> keyboard_ptr(keyboard.get_keyboard());
    for (unsigned key = 0; key != 88; ++ key)
    {
        keyboard_ptr->at(key) = 1 / gain / 87.0 * key;
    }
}