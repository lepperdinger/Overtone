Overtone
========

A Music Visualizer

### Description

Overtone is a software written for GNU/Linux distributions that visualizes
music via projecting its frequency spectrum onto a piano keyboard. If an
individual sound isn't a pure sine wave, it also consists of overtones besides
its fundamental frequency (see [Overtone](https://en.wikipedia.org/wiki/Overtone)).
So if a single note is being played on a piano, Overtone also detects its
corresponding overtones, e.g., the same note one octave higher.

### Building

Overtone can be built via
```
mkdir build
cd build
cmake ..
make -j
```

Overtone requires [FFmpeg](https://ffmpeg.org/about.html) for converting audio
files and saving videos into MP4 files. For Debian-based distributions it can
be usually installed via
```
sudo apt install ffmpeg
```
I recommend downloading and building the
[source code of the latest version of FFmpeg](https://ffmpeg.org/download.html)
because I've experienced problems with older versions, e.g., some players were
not able to play the videos created by older versions.

### Usage
```
$ ./Overtone --help
Overtone: A Music Visualizer (version 0.2.0)

Usage: Overtone [options]... <input file> <output file *.mp4>

  -c <channel>           use a specific audio channel instead of all channels
                         (e.g., 0)
  -f <frame rate>        frame rate in frames per seconds (default = 25)
  -F <FFmpeg executable> path of the FFmpeg executable
  -g <gain>              multiplies each key of the keyboard by this value
                         (default = 35)
  -G <gate>              all keys below this threshold are set to 0
                         (0.0 <= gate <= 1.0) (default = 0)
  -h, --help             show this help message and exit
  -s <history speed>     speed of the history in pixels per video frame
                         (default = 10)
  -t <theme>             theme (default = cyan)

Available themes:
  -> vintage
  -> fire
  -> purple
  -> cyan
  -> matrix
  -> white
  -> gray
```

### Examples
#### Some slow/fast, high/low arpeggios to test Overtone (theme: cyan)
[![arpeggios youtube video](https://user-images.githubusercontent.com/69904414/158032716-8762aa06-45b1-487b-b673-10d1b3ed5d2e.png)](https://youtu.be/6hZ9lliG8Ss)
#### A piano song (theme: fire)
[![paradise youtube video](https://user-images.githubusercontent.com/69904414/158032714-fe166922-eb9c-4b58-bf59-d6952280933a.png)](https://youtu.be/boK7MFoGk6k)
#### The Matrix soundtrack (theme: matrix)
[![matrix theme youtube video](https://user-images.githubusercontent.com/69904414/158032700-7fa197fa-5e33-4bd1-b43c-f4482a492887.png)](https://youtu.be/fQR3fhe0eH8)
