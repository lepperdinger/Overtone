/******************************************************************************

    Overtone: A Music Visualizer

    VideoFrame.h

    Copyright (C) 2022 Stefan Lepperdinger

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef OVERTONE_VIDEOFRAME_H
#define OVERTONE_VIDEOFRAME_H

#include "ColorMap.h"
#include "FFmpeg.h"
#include "Keyboard.h"
#include <string>
#include <vector>

class VideoFrame {
public:
  /**
   * @param ffmpeg
   * @param gain multiplies each key of the keyboard by this value
   * @param gate all keys below this threshold are set to 0
   *             (0.0 <= gate <= 1.0)
   * @param theme name of the color theme
   * @param history_speed speed of the history in pixel rows per video frame
   * @param keyboard
   */
  VideoFrame(FFmpeg ffmpeg, double gain, double gate, std::string theme,
             unsigned history_speed, Keyboard keyboard);

  /**
   * Evaluates the current video frame.
   * @param frame_index saves the video frame into frame_index.png
   * @return false if it's the last frame of the video
   */
  bool evaluate_frame(const unsigned &frame_index);

private:
  using Vector = std::vector<double>;
  using VectorSize = Vector::size_type;
  using Pixel = std::vector<unsigned char>;
  using Row = std::vector<Pixel>;
  using RowSize = Row::size_type;
  using Frame = std::vector<Row>;
  using FrameSize = Frame::size_type;
  Frame frame;
  Row tmp_row;

  // speed of the history in pixel rows per video frame
  unsigned history_speed;

  FFmpeg ffmpeg;
  unsigned frame_width;
  unsigned frame_height;

  // indices of the white_keys
  const std::vector<VectorSize> white_keys;

  // indices of the black keys
  const std::vector<VectorSize> black_keys;

  // RGB color of the current pixel
  unsigned char red, green, blue;

  Keyboard keyboard;
  ColorMap color_map;

  void initialize_video_frame();
  inline void set_pixel(const FrameSize &row, const FrameSize &column);
  inline void set_color(double input_value);
  inline void set_edge_color();
  inline void layer_0_background();
  inline void layer_1_frame();
  inline void layer_2_history();
  inline void layer_3_white_keys();
  inline void layer_4_black_keys();
  inline void layer_5_horizontal_separator();

  /**
   * Saves the current video frame into the file frame_index.png.
   * @param frame_index
   */
  void save_frame(const unsigned &frame_index);
};

#endif // OVERTONE_VIDEOFRAME_H
