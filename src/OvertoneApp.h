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
    OvertoneApp(const std::string &input_file_path,
                const std::string &audio_file_path,
                const std::string &frames_directory_path,
                const std::string &video_path,
                const std::string &ffmpeg_executable_path,
                unsigned frame_rate,
                const std::vector<unsigned> &channels,
                double gain);
private:
    FFmpeg ffmpeg;
    WAVE wave;
};


#endif //OVERTONE_OVERTONEAPP_H
