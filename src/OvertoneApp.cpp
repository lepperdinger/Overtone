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
#include <vector>
#include <sys/stat.h>
#include <iomanip>

using std::cout;
using std::cerr;
using std::endl;

OvertoneApp::OvertoneApp(int argc, char **argv)
{
    frame_rate = 25;
    gain = 35.;
    history_speed = 10;
    gate = 0;
    ffmpeg_executable_path = "ffmpeg";
    for (int index = 0; index != argc; ++index)
    {
        arguments.emplace_back(argv[index]);
    }
    parse_arguments();
    create_temporary_directory();
    evaluate_the_file_paths();
    create_frames_directory();
}

OvertoneApp::~OvertoneApp()
{
    delete_temporary_files();
}

void OvertoneApp::show_help_message() const
{
    std::string usage = "Overtone [options]... <input file path> "
                        "<output file path *.mp4>";

    std::stringstream descriptions_stream;
    descriptions_stream << std::left;
    int argument_length{25};
    std::string new_line = '\n' + std::string(argument_length, ' ');
    descriptions_stream
        << std::setw(argument_length)
        << "  -c <channel>"
        << "use a specific audio channel instead of all channels"
        << new_line
        << "(e.g., 0)\n"

        << std::setw(argument_length)
        << "  -f <frame rate>"
        << "frame rate in frames per seconds (default = "
        << frame_rate << ")\n"

        << std::setw(argument_length)
        << "  -F <FFmpeg executable>"
        << "path of the FFmpeg executable\n"

        << std::setw(argument_length)
        << "  -g <gain>"
        << "gain (default = "
        << gain << ")\n"

        << std::setw(argument_length)
        << "  -G <gate>"
        << "gate (0.0 <= gate <= 1.0) (default = "
        << gate << ")\n"

        << std::setw(argument_length)
        << "  -h, --help"
        << "show this help message and exit\n"

        << std::setw(argument_length)
        << "  -s <history speed>"
        << "speed of the history in lines per frame (default = "
        << history_speed << ")";

    std::string descriptions = descriptions_stream.str();
    cout << usage << endl << endl;
    cout << descriptions << endl;
}

void OvertoneApp::parse_arguments()
{
    if (arguments.size() == 1)
    {
        show_help_message();
        std::exit(EXIT_FAILURE);
    }
    std::vector<std::string> positional_arguments;
    for (auto argument = ++arguments.cbegin(); argument != arguments.cend();
         ++argument)
    {
        if (*argument == "-c")
        {
            unsigned channel = parse_argument(argument,
                                              &OvertoneApp::to_unsigned,
                                              true,
                                              false,
                                              true);
            channels = {channel};
        }
        else if (*argument == "-f")
        {
            frame_rate = parse_argument(argument,
                                        &OvertoneApp::to_unsigned,
                                        true,
                                        true,
                                        true);
        }
        else if (*argument == "-F")
        {
            ffmpeg_executable_path = parse_argument(argument,
                                                    &OvertoneApp::to_string,
                                                    false,
                                                    false,
                                                    false);
        }
        else if (*argument == "-g")
        {
            gain = parse_argument(argument,
                                  &OvertoneApp::to_double,
                                  true,
                                  false,
                                  false);
        }
        else if (*argument == "-G")
        {
            gate = parse_argument(argument,
                                  &OvertoneApp::to_double,
                                  true,
                                  false,
                                  false);
        }
        else if (*argument == "-h" || *argument == "--help")
        {
            show_help_message();
            std::exit(EXIT_SUCCESS);
        }
        else if (*argument == "-s")
        {
            history_speed = parse_argument(argument,
                                           &OvertoneApp::to_unsigned,
                                           true,
                                           true,
                                           true);
        }
        else if ((*argument)[0] == '-')
        {
            std::string error_message = "Error: unrecognized argument: "
                                        + *argument;
            cerr << error_message << endl;
            std::exit(EXIT_FAILURE);
        }
        else
        {
            positional_arguments.push_back(*argument);
        }
    }
    unsigned number_of_positional_arguments = 2;
    if (positional_arguments.size() != number_of_positional_arguments)
    {
        cout << "Error: the following arguments are required: "
             << "<input file path> <output file path *.mp4>" << endl;
        std::exit(EXIT_FAILURE);
    }
    else
    {
        input_file_path.assign(positional_arguments[0]);
        video_path.assign(positional_arguments[1]);
        std::ifstream video_file(video_path);
        bool video_file_exists = video_file.good();
        if (video_file_exists)
        {
            cerr << "Error: The file '" + video_path + "' does already exist."
                 << endl;
            std::exit(EXIT_FAILURE);
        }
        else
        {
            video_file.close();
        }
        if (!(video_path.size() >= 4
                && std::string(video_path.cend() - 4, video_path.cend())
                == ".mp4"))
        {
            cerr << "Error: The name of the output file has to end with '.mp4'."
                 << endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

template <typename T>
T OvertoneApp::parse_argument(
    std::vector<std::string>::const_iterator &current_argument,
    T (*conversion)(const std::string &),
    bool is_positive,
    bool is_nonzero,
    bool is_integer
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
        std::exit(EXIT_FAILURE);
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
            std::exit(EXIT_FAILURE);
        }
        if (is_integer)
        {
            auto search_result = std::find(current_argument->cbegin(),
                                           current_argument->cend(),
                                           '.');
            if (search_result != current_argument->cend())
            {
                cerr << "Error: argument "
                     << flag
                     << " : The value has to be an integer."
                     << endl;
                std::exit(EXIT_FAILURE);
            }
        }        if (is_positive && (*current_argument)[0] == '-')
        {
            cerr << "Error: argument "
                 << flag
                 << " : The value has to be a positive number."
                 << endl;
            std::exit(EXIT_FAILURE);
        }
        if (is_nonzero && (*current_argument)[0] == '0')
        {
            cerr << "Error: argument "
                 << flag
                 << " : The value has to be nonzero."
                 << endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return parsed_value;
}

void OvertoneApp::create_temporary_directory()
{
    char directory_template[] = "/tmp/Overtone.XXXXXX";
    char *tmp_directory = mkdtemp(directory_template);
    if (tmp_directory == nullptr)
    {
        cerr << "The creation of the temporary directory failed." << endl;
        std::exit(EXIT_FAILURE);
    }
    else
    {
        temporary_directory = std::string(tmp_directory);
    }
}

void OvertoneApp::evaluate_the_file_paths()
{
    audio_file_path = temporary_directory + "/audio.wav";
    frames_directory_path = temporary_directory + "/frames";
}

void OvertoneApp::create_frames_directory()
{
    if (mkdir(frames_directory_path.c_str(), 0775))
    {
        cerr << "Error: The directory '" << frames_directory_path
             << "' does already exist. Please remove, move, or rename this "
                "directory."
             << endl;
        std::exit(EXIT_FAILURE);
    }
}

void OvertoneApp::convert_input_file_to_wav()
{
    try
    {
        ffmpeg = FFmpeg(input_file_path,
                        audio_file_path,
                        frames_directory_path,
                        video_path,
                        ffmpeg_executable_path,
                        frame_rate);
    }
    catch (const std::exception &exception)
    {
        cerr << "Overtone: Error: " << exception.what() << endl;
        delete_temporary_files();
        std::exit(EXIT_FAILURE);
    }

     // Converting the input file to a WAVE file via FFmpeg
    try
    {
        ffmpeg.convert_to_wave();
    }
    catch(const FFmpeg::file_conversion_error &file_conversion_error)
    {
        cout << file_conversion_error.what() << endl;
        delete_temporary_files();
        std::exit(EXIT_FAILURE);
    }
}

void OvertoneApp::decode_wav_file()
{
    wave = WAVE(audio_file_path);
}

void OvertoneApp::initialize_the_keyboard() {
    try
    {
        keyboard = Keyboard({
                Spectrum(wave, channels, frame_rate, {0, 11}, 67000),
                Spectrum(wave, channels, frame_rate, {11, 22}, 44000),
                Spectrum(wave, channels, frame_rate, {22, 33}, 29000),
                Spectrum(wave, channels, frame_rate, {33, 46}, 15500),
                Spectrum(wave, channels, frame_rate, {46, 56}, 8500),
                Spectrum(wave, channels, frame_rate, {56, 74}, 5000),
                Spectrum(wave, channels, frame_rate, {74, 81}, 2500),
                Spectrum(wave, channels, frame_rate, {81, 88}, 1900)
        });
    }
    catch (const std::exception &exception)
    {
        cerr << "Overtone: Error: " << exception.what() << endl;
        delete_temporary_files();
        std::exit(EXIT_FAILURE);
    }
}

void OvertoneApp::create_the_video()
{
    try
    {
        VideoFrame video_frame = VideoFrame(ffmpeg,
                                            gain,
                                            gate,
                                            history_speed,
                                            keyboard);
        unsigned frame = 0;
        while (video_frame.evaluate_frame(frame))
        {
            cout << "current frame: " << frame << "          \r" << std::flush;
            ++frame;
        }
        cout << endl;
    }
    catch (const std::exception &exception)
    {
        cerr << "Overtone: Error: " << exception.what() << endl;
        delete_temporary_files();
        std::exit(EXIT_FAILURE);
    }

    ffmpeg.convert_to_mp4();
}

void OvertoneApp::delete_temporary_files()
{
    std::string command = "rm -r " + temporary_directory;
    std::system(command.c_str());
}

void OvertoneApp::run()
{
    convert_input_file_to_wav();
    decode_wav_file();
    initialize_the_keyboard();
    create_the_video();
}
