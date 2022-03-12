/******************************************************************************

    Overtone: A Music Visualizer

    Tests.h

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

#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H

#define TEST(condition)                                                       \
  {                                                                           \
    if (condition)                                                            \
      std::cerr << "[\033[1;32m PASSED \033[0m]\033[0;36m file \033[0m"       \
                << __FILE__ << "\033[0;36m at line \033[0m" << __LINE__       \
                << std::endl;                                                 \
    else                                                                      \
      std::cerr << "[\033[1;31m FAILED \033[0m]\033[0;36m file \033[0m"       \
                << __FILE__ << "\033[0;36m at line \033[0m" << __LINE__       \
                << std::endl;                                                 \
  }

#include <iostream>

class Tests
{
public:
  static void run_all_tests ();
};

#endif // TESTS_TESTS_H
