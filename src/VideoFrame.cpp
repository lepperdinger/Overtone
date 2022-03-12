//
// Created by stefan on 8/23/21.
//
#include "VideoFrame.h"
#include <iomanip>
#include <iostream>

VideoFrame::VideoFrame (FFmpeg ffmpeg, double gain, double gate,
                        std::string theme, unsigned history_speed,
                        Keyboard keyboard)
    : frame (), tmp_row (), history_speed (history_speed),
      ffmpeg (std::move (ffmpeg)), frame_width (1920), frame_height (1080),
      white_keys ({ 0,  2,  3,  5,  7,  8,  10, 12, 14, 15, 17, 19, 20,
                    22, 24, 26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43,
                    44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65,
                    67, 68, 70, 72, 74, 75, 77, 79, 80, 82, 84, 86, 87 }),
      black_keys ({ 1,  4,  6,  9,  11, 13, 16, 18, 21, 23, 25, 28,
                    30, 33, 35, 37, 40, 42, 45, 47, 49, 52, 54, 57,
                    59, 61, 64, 66, 69, 71, 73, 76, 78, 81, 83, 85 }),
      red (0), green (0), blue (0), keyboard (std::move (keyboard)),
      color_map (std::move (theme), gain, gate)
{
  if (history_speed == 0 || history_speed > 786)
    {
      throw std::out_of_range (
          "The argument `history_speed` is not within the interval [1, 786].");
    }
  initialize_video_frame ();
  layer_0_background ();
  layer_1_frame ();
}

void
VideoFrame::initialize_video_frame ()
{
  for (FrameSize row_index = 0; row_index < frame_height; ++row_index)
    {
      Pixel pixel ({ 0, 0, 0 });
      Row row;
      for (RowSize column_index = 0; column_index < frame_width;
           ++column_index)
        {
          row.push_back (pixel);
        }
      frame.push_back (row);
    }
  Pixel pixel ({ 0, 0, 0 });
  Row row;
  for (RowSize column_index = 0; column_index < frame_width; ++column_index)
    {
      tmp_row.push_back (pixel);
    }
}

bool
VideoFrame::evaluate_frame (const unsigned &frame_index)
{
  layer_2_history ();
  layer_3_white_keys ();
  layer_4_black_keys ();
  layer_5_horizontal_separator ();
  save_frame (frame_index);
  return keyboard.go_to_next_frame ();
}

void
VideoFrame::save_frame (const unsigned &frame_index)
{
  std::string ppm_file_path =
      ffmpeg.get_frames_directory_path () + "/current_frame.ppm";
  std::ofstream ppm_file (ppm_file_path);
  if (!ppm_file)
    {
      throw std::invalid_argument ("Can't access '" + ppm_file_path + "'.");
    }
  ppm_file << "P6\n"
           << frame_width << ' ' << frame_height << '\n'
           << "255" << '\n';
  for (FrameSize row = 0; row < frame_height; ++row)
    for (RowSize column = 0; column < frame_width; ++column)
      {
        ppm_file << frame[row][column][0] << frame[row][column][1]
                 << frame[row][column][2];
      }
  ppm_file << std::flush;

  std::ostringstream png_file_path;
  png_file_path << ffmpeg.get_frames_directory_path () << "/"
                << std::setfill ('0') << std::setw (7) << frame_index
                << ".png";
  std::string command = "'" + ffmpeg.get_ffmpeg_executable_path () + "' -i '"
                        + ppm_file_path + "' '" + png_file_path.str ()
                        + "' -y 1>/dev/null 2>&1";
  if (std::system (command.c_str ()))
    {
      throw FFmpeg::file_conversion_error ("Conversion from '" + ppm_file_path
                                           + "' to '" + png_file_path.str ()
                                           + " failed.");
    }
}

inline void
VideoFrame::set_pixel (const FrameSize &row, const FrameSize &column)
{
  frame[row][column].assign ({ red, green, blue });
}

void
VideoFrame::set_color (double input_value)
{
  auto rgb_color = color_map (input_value);
  red = rgb_color[0];
  green = rgb_color[1];
  blue = rgb_color[2];
}

void
VideoFrame::set_edge_color ()
{
  auto rgb_color = color_map.get_edge_color ();
  red = rgb_color[0];
  green = rgb_color[1];
  blue = rgb_color[2];
}

void
VideoFrame::layer_0_background ()
{
  set_color (0);
  for (FrameSize row = 0; row < frame_height; ++row)
    {
      for (RowSize column = 0; column < frame_width; ++column)
        {
          frame[row][column].assign ({ red, green, blue });
        }
    }
}

void
VideoFrame::layer_1_frame ()
{
  set_edge_color ();
  for (FrameSize row = 0; row != frame_height; ++row)
    {
      for (FrameSize column = 0; column != 24; ++column)
        {
          set_pixel (row, column);
        }
      for (FrameSize column = frame_width - 24; column != frame_width;
           ++column)
        {
          set_pixel (row, column);
        }
    }
  for (FrameSize column = 0; column != frame_width; ++column)
    {
      for (FrameSize row = 0; row != 24; ++row)
        {
          set_pixel (row, column);
        }
      for (FrameSize row = frame_height - 24; row != frame_height; ++row)
        {
          set_pixel (row, column);
        }
    }
}

inline void
VideoFrame::layer_2_history ()
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

void
VideoFrame::layer_3_white_keys ()
{
  FrameSize column = 24;
  for (VectorSize white_key : white_keys)
    {
      set_edge_color ();
      for (FrameSize column_counter = 0; column_counter != 2; ++column_counter)
        {
          for (FrameSize row = 822; row != 1056; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }
      set_color ((*keyboard.get_keyboard ())[white_key]);
      for (FrameSize column_counter = 0; column_counter != 32;
           ++column_counter)
        {
          for (FrameSize row = 809; row != 1056; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }
      set_edge_color ();
      for (FrameSize column_counter = 0; column_counter != 2; ++column_counter)
        {
          for (FrameSize row = 822; row != 1056; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }
    }
}

void
VideoFrame::layer_4_black_keys ()
{
  FrameSize column = 51;

  unsigned c_sharp = 0;
  unsigned f_sharp = 2;
  unsigned a_sharp = 4;

  unsigned note = a_sharp;

  for (VectorSize key : black_keys)
    {
      if (note == c_sharp || note == f_sharp)
        {
          // There are no black keys between H & C and E & F.
          column += 36;
          ++note;
        }
      else if (note == a_sharp)
        {
          note = c_sharp;
        }
      else
        {
          ++note;
        }

      set_edge_color ();

      // Left black line
      for (FrameSize column_counter = 0; column_counter != 4; ++column_counter)
        {
          for (FrameSize row = 822; row != 980; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }

      column -= 4;

      // Left side of the first row of the history
      set_color (0);
      {
        FrameSize row = 809;
        for (FrameSize column_counter = 0; column_counter != 4;
             ++column_counter)
          {
            set_pixel (row, column);
            ++column;
          }
      }

      set_color ((*keyboard.get_keyboard ())[key]);

      // Colored part in the middle
      for (FrameSize column_counter = 0; column_counter != 10;
           ++column_counter)
        {
          for (FrameSize row = 809; row != 976; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }

      set_edge_color ();

      // Black line at the bottom
      column -= 10;
      for (FrameSize column_counter = 0; column_counter != 10;
           ++column_counter)
        {
          for (FrameSize row = 976; row != 980; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }

      // Right black line
      for (FrameSize column_counter = 0; column_counter != 4; ++column_counter)
        {
          for (FrameSize row = 822; row != 980; ++row)
            {
              set_pixel (row, column);
            }
          ++column;
        }

      column -= 4;
      // Left side of the first row of the history
      set_color (0);
      {
        FrameSize row = 809;
        for (FrameSize column_counter = 0; column_counter != 4;
             ++column_counter)
          {
            set_pixel (row, column);
            ++column;
          }
      }

      column += 18;
    }
}

void
VideoFrame::layer_5_horizontal_separator ()
{
  set_edge_color ();
  for (FrameSize row = 810; row != 822; ++row)
    {
      for (FrameSize column = 0; column != frame_width; ++column)
        {
          set_pixel (row, column);
        }
    }
}
