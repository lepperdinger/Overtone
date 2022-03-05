//
// Created by stefan on 8/23/21.
//

#ifndef OVERTONE_VIDEOFRAME_H
#define OVERTONE_VIDEOFRAME_H

#include "ColorMap.h"
#include "FFmpeg.h"
#include "Keyboard.h"
#include <string>
#include <vector>

class VideoFrame
{
public:
  using Vector = std::vector<double>;
  using VectorSize = Vector::size_type;
  VideoFrame (FFmpeg ffmpeg, double gain, double gate, std::string theme,
              unsigned history_speed, Keyboard keyboard);
  void save_frame (const unsigned &frame_index);
  bool evaluate_frame (const unsigned &frame_index);

private:
  using Pixel = std::vector<unsigned char>;
  using Row = std::vector<Pixel>;
  using RowSize = Row::size_type;
  using Frame = std::vector<Row>;
  using FrameSize = Frame::size_type;
  Frame frame;
  Row tmp_row;
  unsigned history_speed;
  FFmpeg ffmpeg;
  unsigned frame_width;
  unsigned frame_height;
  const std::vector<VectorSize> white_keys;
  const std::vector<VectorSize> black_keys;
  unsigned char red, green, blue;
  Keyboard keyboard;
  ColorMap color_map;

  void create_frame ();
  inline void set_pixel (const FrameSize &row, const FrameSize &column);

  inline void set_color (double input_value);
  inline void set_edge_color ();
  inline void layer_0_background ();
  inline void layer_1_frame ();
  inline void layer_2_history ();
  inline void layer_3_white_keys ();
  inline void layer_4_black_keys ();
  inline void layer_5_horizontal_separator ();
};

#endif // OVERTONE_VIDEOFRAME_H
