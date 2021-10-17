//
// Created by stefan on 8/13/21.
//

#ifndef OVERTONE_OVERTONEAPP_H
#define OVERTONE_OVERTONEAPP_H


#include "FFmpeg.h"
#include "WAVE.h"
#include "Spectrum.h"
#include "Keyboard.h"

class OvertoneApp {
public:
    explicit OvertoneApp(std::vector<std::string> arguments);
    void run();
private:
    /**
     * Splits the string at the last occurrence of the separator and throws a
     * runtime error if the string doesn't contain the separator.
     * @param string The spring to be split.
     * @param separator The character at which the function splits the string.
     * @return A pair that contains the string before and the string after
     *         the separator.
     */
    static std::pair<std::string, std::string> split(const std::string &string,
                                                     const char &separator);
    void parse_arguments();
    int parse_integer_argument(
        std::vector<std::string>::const_iterator &current_argument
    );
    std::string parse_string_argument(
        std::vector<std::string>::const_iterator &current_argument
    );
    double parse_double_argument(
        std::vector<std::string>::const_iterator &current_argument
    );
    void evaluate_the_file_paths();
    void create_frames_directory();
    void convert_input_file_to_wav();
    void decode_wav_file();
    void initialize_the_keyboard();
    void create_the_video();

    std::vector<std::string> arguments;

    // path of the FFmpeg executable
    std::string ffmpeg_executable_path;

    // path of the input file
    std::string input_file_path;

    // path of the directory that contains the input file
    std::string input_directory_path;

    std::string input_filename_with_extension;

    // name of the input file (without its file extension)
    std::string input_filename;

    // file extension of the input file
    std::string file_extension;

    // path of the audio file that will be created fy FFmpeg
    std::string audio_file_path;

    std::string frames_directory_path;

    std::string video_path;

    unsigned frame_rate;

    double gain;

    unsigned history_speed;

    WAVE wave;

    FFmpeg ffmpeg;

    Keyboard keyboard;

    std::vector<unsigned> channels;
};


#endif //OVERTONE_OVERTONEAPP_H
