//
// Created by stefan on 8/15/21.
//

#ifndef OVERTONE_WAVE_H
#define OVERTONE_WAVE_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * Open and decodes a WAVE file that contains a 16 bit linear-PCM signal
 * (signed and little endian).
 */
class WAVE
{
public:
  /**
   * A parsing exception occurs if the parsing of the WAVE file fails, e.g.,
   * due to an unsupported bit depth or if the file isn't a WAVE file.
   */
  class parsing_error : public std::runtime_error
  {
  public:
    explicit parsing_error (const std::string &message)
        : std::runtime_error (message)
    {
    }
  };

  ~WAVE () = default;

  WAVE () = default;

  /**
   * Decodes the WAVE file `audio_file_path`.
   * @param audio_file_path path of the WAVE file
   */
  explicit WAVE (std::string audio_file_path)
  try : audio_file_path (std::move (audio_file_path))
    {
      decode ();
    }
  catch (parsing_error &parsing_error)
    {
      std::cerr << parsing_error.what () << std::endl;
    }

  /**
   * Shows information about the WAVE file, e.g., bit rate, bit depth,
   * number of channels, ...
   */
  void show_file_contents () const;

  /**
   * Returns the audio signal.
   * @return audio signal
   */
  std::vector<std::shared_ptr<std::vector<double> > >
  get_signal () const
  {
    return data;
  }

  /**
   * Returns the sample rate.
   * @return sample rate
   */
  const unsigned int &
  get_sample_rate () const
  {
    return sample_rate;
  }

private:
  void decode ();
  static void read (std::ifstream &file, uint16_t &destination);
  static void read (std::ifstream &file, int16_t &destination);
  static void read (std::ifstream &file, uint32_t &destination);
  static void read (std::ifstream &file, std::string &destination);
  static void search_chunk (std::ifstream &file,
                            const std::string &wanted_chunk_id,
                            uint32_t &found_chunk_size);
  void parse_riff_chunk_head (std::ifstream &file);
  void parse_format_chunk (std::ifstream &file);
  void parse_data_chunk (std::ifstream &file);

  std::string audio_file_path;

  // RIFF chunk
  std::string chunk_id{};
  uint32_t chunk_size{};
  std::string format{};

  // format sub-chunk
  std::string format_chunk_id{};
  uint32_t format_chunk_size{};
  uint16_t audio_format{};
  uint16_t number_of_channels{};
  uint32_t sample_rate{};
  uint32_t byte_rate{};
  uint16_t block_align{};
  uint16_t bits_per_sample{};

  // data sub-chunk
  std::string data_chunk_id{};
  uint32_t data_chunk_size{};
  // Each uint16_t vector contains the data of a channel respectively
  // (Mono = 1 Channel, Stereo = 2 Channels, ...)
  std::vector<std::shared_ptr<std::vector<double> > > data;
};

#endif // OVERTONE_WAVE_H
