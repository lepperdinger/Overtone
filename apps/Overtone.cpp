/******************************************************************************

    Overtone: A Music Visualizer

    Overtone.cpp

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

#include "OvertoneApp.h"
#include <csignal>
#include <memory>

std::unique_ptr<OvertoneApp> overtone_app;

void signal_handler(int signal) {
  if (signal == 2) {
    std::cerr << "\nReceived the interrupt signal..." << std::endl;
  }
  std::exit(1);
}

int main(int argc, char **argv) {
  signal(SIGINT, signal_handler);
  overtone_app = std::make_unique<OvertoneApp>(argc, argv);
  overtone_app->run();
}
