//
// Created by stefan on 8/23/21.
//
#include <iostream>
#include <iomanip>
#include "VideoFrame.h"

using std::cout;
using std::cerr;
using std::endl;

VideoFrame::VideoFrame(FFmpeg ffmpeg,
                       double gain,
                       unsigned history_speed,
                       Keyboard keyboard):
    frame(),
    tmp_row(),
    history_speed(history_speed),
    ffmpeg(std::move(ffmpeg)),
    frame_width(1920),
    frame_height(1080),
    gain(gain),
    white_keys({0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27,
                29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55,
                56, 58, 60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82,
                84, 86, 87}),
    black_keys({1, 4, 6, 9, 11, 13, 16, 18, 21, 23, 25, 28, 30, 33, 35, 37, 40,
                42, 45, 47, 49, 52, 54, 57, 59, 61, 64, 66, 69, 71, 73, 76, 78,
                81, 83, 85}),
    red(0),
    green(0),
    blue(0),
    keyboard(std::move(keyboard))
{
    if (gain < 0)
    {
        throw std::out_of_range("The argument `gain` is negative.");
    }

    if (history_speed == 0 || history_speed > 786)
    {
        throw std::out_of_range("The argument `history_speed` is not within "
                                "the interval [1, 786].");
    }
    create_frame();
    layer_0_background();
    layer_1_frame();
}

void VideoFrame::create_frame()
{
    for (FrameSize row_index = 0; row_index < frame_height; ++row_index)
    {
        Pixel pixel({0, 0, 0});
        Row row;
        for (RowSize column_index = 0; column_index < frame_width;
             ++column_index)
        {
            row.push_back(pixel);
        }
        frame.push_back(row);
    }
    Pixel pixel({0, 0, 0});
    Row row;
    for (RowSize column_index = 0; column_index < frame_width;
         ++column_index)
    {
        tmp_row.push_back(pixel);
    }
}

bool VideoFrame::evaluate_frame(const unsigned &frame_index)
{
    layer_2_history();
    layer_3_white_keys();
    layer_4_black_keys();
    layer_5_horizontal_separator();
    save_frame(frame_index);
    return keyboard.go_to_next_frame();
}

void VideoFrame::save_frame(const unsigned &frame_index)
{
    std::string ppm_file_path = ffmpeg.get_frames_directory_path()
                                + "/current_frame.ppm";
    std::ofstream ppm_file(ppm_file_path);
    if (!ppm_file)
    {
        throw std::invalid_argument("Can't access '" + ppm_file_path + "'.");
    }
    ppm_file << "P6\n"
             << frame_width << ' ' << frame_height << '\n'
             << "255" << '\n';
    for (FrameSize row = 0; row < frame_height; ++row)
        for(RowSize column = 0; column < frame_width; ++column)
        {
            ppm_file << frame[row][column][0]
                     << frame[row][column][1]
                     << frame[row][column][2];
        }
    ppm_file << std::flush;

    std::ostringstream png_file_path;
    png_file_path << ffmpeg.get_frames_directory_path() << "/"
                  << std::setfill('0') << std::setw(7)
                  << frame_index << ".png";
    std::string command = ffmpeg.get_ffmpeg_executable_path()
                          + " -i " + ppm_file_path
                          + " " + png_file_path.str()
                          + " -y 1>/dev/null 2>&1";
    if (std::system(command.c_str()))
    {
        throw FFmpeg::file_conversion_error("Conversion from '" + ppm_file_path
                                            + "' to '" + png_file_path.str()
                                            + " failed.");
    }
}

inline void VideoFrame::set_pixel(const FrameSize &row,
                                  const FrameSize &column)
{
    frame[row][column].assign({red, green, blue});
}

void VideoFrame::color_map(double input_value)
{
#ifdef DEBUG
    if (input_value < 0)
    {
        throw std::out_of_range("The argument `input_value` is negative.");
    }
#endif

    input_value *= 255.0 * gain;

    if (input_value > 255)
    {
        input_value = 255.0;
    }

    if (input_value < 85)
    {
        double scaled = input_value / 85.;
        red   = 43. + scaled * 8.;
        green = 44. + scaled * 109.;
        blue  = 40. + scaled * 97;
    }
    else if (input_value < 170)
    {
        double scaled = (input_value - 85) / 85;
        red   = 51.  + scaled * 74;
        green = 153. + scaled * 73;
        blue  = 137. + scaled * 72;
    }
    else {
        double scaled = (input_value - 170) / 85;
        red   = 125. + scaled * 130;
        green = 226. + scaled * 24;
        blue  = 209. + scaled * 42;
    }
}


void VideoFrame::layer_0_background()
{
    color_map(0);
    auto frame_iterator = frame.begin();
    auto frame_end= frame.end();
    for (FrameSize row = 0; row < frame_height; ++row)
    {
        for (RowSize column = 0; column < frame_width; ++column)
        {
            frame[row][column].assign({red, green, blue});
        }
    }
}

void VideoFrame::layer_1_frame()
{
    red = 0;
    green = 0;
    blue = 0;
    for (FrameSize row = 0; row != frame_height; ++ row)
    {
        for (FrameSize column = 0; column != 24; ++ column)
        {
            set_pixel(row, column);
        }
        for (FrameSize column = frame_width - 24; column != frame_width;
             ++column)
        {
            set_pixel(row, column);
        }
    }
    for (FrameSize column = 0; column != frame_width; ++column)
    {
        for (FrameSize row = 0; row != 24; ++row)
        {
            set_pixel(row, column);
        }
        for (FrameSize row = frame_height - 24; row != frame_height; ++row)
        {
            set_pixel(row, column);
        }
    }
}

void VideoFrame::layer_5_horizontal_separator()
{
    red = 0;
    green = 0;
    blue = 0;
    for (FrameSize row = 810; row != 822; ++ row)
    {
        for (FrameSize column = 0; column != frame_width; ++ column)
        {
            set_pixel(row, column);
        }
    }
}

void VideoFrame::layer_3_white_keys()
{
    FrameSize column = 24;
    for (VectorSize white_key : white_keys)
    {
        red = 0;
        green = 0;
        blue = 0;
        for (FrameSize column_counter = 0; column_counter != 2;
             ++column_counter)
        {
            for (FrameSize row = 822; row != 1056; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }
#ifdef DEBUG
        color_map(keyboard.get_keyboard()->at(white_key));
#else
        color_map((*keyboard.get_keyboard())[white_key]);
#endif
        for (FrameSize column_counter = 0; column_counter != 32;
             ++column_counter)
        {
            for (FrameSize row = 809; row != 1056; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }
        red = 0;
        green = 0;
        blue = 0;
        for (FrameSize column_counter = 0; column_counter != 2;
             ++column_counter)
        {
            for (FrameSize row = 822; row != 1056; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }
    }
}

void VideoFrame::layer_4_black_keys()
{
//    for (FrameSize row = 809; row != 980; ++row)

    FrameSize column = 51;

    // 0 = C#
    // 1 = D#
    // 2 = F#
    // 3 = G#
    // 4 = A#
    unsigned note = 4;

    for (VectorSize key : black_keys)
    {
        if (note == 0 || note == 2)
        {
            column += 36;
            ++note;
        }
        else if (note == 4)
        {
            note = 0;
        }
        else
        {
            ++note;
        }

        red = 0;
        green = 0;
        blue = 0;

        // Left black line
        for (FrameSize column_counter = 0; column_counter != 4;
             ++column_counter)
        {
            for (FrameSize row = 822; row != 980; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }

        column -= 4;

        // Left side of the first row of the history
        color_map(0);
        {
            FrameSize row = 809;
            for (FrameSize column_counter = 0; column_counter != 4;
                 ++column_counter)
            {
                    set_pixel(row, column);
                ++column;
            }
        }

#ifdef DEBUG
        color_map(keyboard.get_keyboard()->at(key));
#else
        color_map((*keyboard.get_keyboard())[key]);
#endif

        // Colored part in the middle
        for (FrameSize column_counter = 0; column_counter != 10;
             ++column_counter)
        {
            for (FrameSize row = 809; row != 976; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }

        red = 0;
        green = 0;
        blue = 0;

        // Black line at the bottom
        column -= 10;
        for (FrameSize column_counter = 0; column_counter != 10;
             ++column_counter)
        {
            for (FrameSize row = 976; row != 980; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }

        // Right black line
        for (FrameSize column_counter = 0; column_counter != 4;
             ++column_counter)
        {
            for (FrameSize row = 822; row != 980; ++row)
            {
                set_pixel(row, column);
            }
            ++column;
        }

        column -= 4;
        // Left side of the first row of the history
        color_map(0);
        {
            FrameSize row = 809;
            for (FrameSize column_counter = 0; column_counter != 4;
                 ++column_counter)
            {
                    set_pixel(row, column);
                ++column;
            }
        }


        column += 18;
    }
}

inline void VideoFrame::layer_2_history()
{
    for (FrameSize row = 809; row != 809 - history_speed + 1; --row)
    {
        frame[row - 1] = frame[row];
    }
    for (FrameSize row = 24; row != 810 - history_speed; ++row)
    {
        frame[row] = frame[row + history_speed];
    }
}