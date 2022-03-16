### Dataflow:

1. `FFmpeg.cpp/.h` converts the input file via FFmpeg into a WAVE file.
2. `WAVE.cpp/.h` decodes the WAVE file.
3. `Spectrum.cpp/.h` evaluates the frequency spectrum.
4. `Keyboard.cpp/.h` projects the frequency spectrum onto the piano keyboard.
5. `VideoFrame.cpp/.h` creates the video frames.
6. `FFmpeg.cpp/.h` saves the video frames and the audio via FFmpeg into a MP4 file.
