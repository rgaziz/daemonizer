// service.h 5/03/2021
/* ========================================================================== */
#pragma once

/* ========================================================================== */
#include <atomic>
#include <mutex>
#include <thread>

/* ========================================================================== */
namespace daemonizer {

/* ========================================================================== */
class Service {
 public:
  explicit Service() : is_wait_(false), is_running_(false) {}
  virtual ~Service() {}

  Service(const Service &) = delete;
  Service &operator=(const Service &) = delete;

 public:
  bool wait(int64_t seconds);
  void detach();
  void start();
  void stop();
  void join();

 protected:
  bool is_running() const;
  virtual bool run() = 0;

 private:
  std::thread th_;
  std::mutex mutex_;
  std::atomic_bool is_wait_;
  std::atomic_bool is_running_;
};
/* ========================================================================== */
};  // namespace new_daemon
/* ========================================================================== */
