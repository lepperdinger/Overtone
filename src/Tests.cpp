//
// Created by stefan on 2/25/22.
//

#include "Tests.h"
#include "ColorMap.h"
#include "Keyboard.h"
#include "Spectrum.h"

void
Tests::run_all_tests ()
{
  ColorMap::run_tests ();
  Keyboard::run_tests();
  Spectrum::run_tests();
}
