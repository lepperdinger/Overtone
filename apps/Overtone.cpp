//
// Created by stefan on 2/25/22.
//
#include "OvertoneApp.h"
#include <csignal>
#include <memory>

std::unique_ptr<OvertoneApp> overtone_app;

void
signal_handler (int signal)
{
  if (signal == 2)
    {
      std::cerr << "\nReceived the interrupt signal..." << std::endl;
    }
  std::exit (1);
}

int
main (int argc, char **argv)
{
  signal (SIGINT, signal_handler);
  overtone_app = std::make_unique<OvertoneApp> (argc, argv);
  overtone_app->run ();
}
