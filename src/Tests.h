//
// Created by stefan on 2/25/22.
//

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
