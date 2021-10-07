//
// Created by stefan on 8/23/21.
//

#ifndef OVERTONE_VIDEOFRAME_H
#define OVERTONE_VIDEOFRAME_H


#include <vector>
#include <string>
#include "Keyboard.h"
#include "FFmpeg.h"

class VideoFrame
{
public:
    using Frame = std::vector<unsigned char>;
    using FrameSize = Frame::size_type;
    using Vector = std::vector<double>;
    using VectorSize = std::vector<double>::size_type;
    VideoFrame(FFmpeg ffmpeg,
               double gain,
               Keyboard keyboard);
    void save_frame(const unsigned &frame_index);
    bool evaluate_frame(const unsigned &frame_index);
private:
    FFmpeg ffmpeg;
    unsigned frame_width;
    unsigned frame_height;
    FrameSize number_of_pixels;
    Frame frame;
    double gain;
    const std::vector<VectorSize> white_keys;
    const std::vector<VectorSize> black_keys;
    unsigned char red, green, blue;
    Keyboard keyboard;

    inline void set_pixel(const FrameSize &row,
                          const FrameSize &column);

    inline void color_map(double input_value);



    inline void layer_0_background();
    inline void layer_1_frame();
    inline void layer_2_history();
    inline void layer_3_white_keys();
    inline void layer_4_black_keys();
    inline void layer_5_horizontal_separator();
};


#endif //OVERTONE_VIDEOFRAME_H
