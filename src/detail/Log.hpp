//
// Created by Michael Wittmann on 06/06/2020.
//

#ifndef SFMLTEST_LOG_HPP
#define SFMLTEST_LOG_HPP

#include <spdlog/logger.h>

#include <memory>  // for shared_ptr

#include "Core.hpp"

namespace app {

class Log {
 public:
  static void Initialize();
  static Ref<spdlog::logger> &logger() { return core_logger_; }

 private:
  static Ref<spdlog::logger> core_logger_;
  static constexpr int max_file_size = 1024 * 1024 * 10;
  static constexpr int max_files = 3;
  static constexpr int max_threads = 1;
  static constexpr int max_items = 8192;
};

}  // namespace app

#endif  // SFMLTEST_LOG_HPP
