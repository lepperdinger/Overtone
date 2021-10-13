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
    std::pair<std::string, std::string> split(const std::string &string,
                                              const char &separator);
    std::vector<std::string> arguments;
};


#endif //OVERTONE_OVERTONEAPP_H
