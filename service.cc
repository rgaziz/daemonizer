// service.cc 5/03/2021
/* ========================================================================== */
#include "service.h"
#include <unistd.h>

/* ========================================================================== */
using namespace daemonizer;

/* ========================================================================== */
bool Service::is_running() const { return is_running_; }

/* ========================================================================== */
void Service::start() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (th_.joinable()) {
    throw std::runtime_error("Service is already running");
  }

  th_ = std::thread(&Service::run, this);
  is_running_ = true;
}

/* ========================================================================== */
void Service::stop() {
  is_running_ = false;
  is_wait_ = false;
  join();
}

/* ========================================================================== */
bool Service::wait(int64_t seconds = 0) {
  is_wait_ = true;

  while (is_wait_ and --seconds >= 0) {
    ::sleep(1);
  }

  return is_wait_;
}

/* ========================================================================== */
void Service::detach() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (th_.joinable()) {
    th_.detach();
  }
}

/* ========================================================================== */
void Service::join() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (th_.joinable()) {
    th_.join();
  }
}

/* ========================================================================== */
