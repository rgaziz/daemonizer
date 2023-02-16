// daemon.h 5/03/2021
/* ========================================================================== */
#pragma once

/* ========================================================================== */
#include <vector>
#include "service.h"

/* ========================================================================== */
namespace daemonizer {

/* ========================================================================== */
using Services = std::vector<std::shared_ptr<Service>>;

/* ========================================================================== */
class Daemonizer {
 public:
  Daemonizer(const int argc, const char **argv);
  virtual ~Daemonizer();

 public:
  void add_service(std::shared_ptr<Service>& service);
  virtual bool initialize() = 0;
  bool run();

 protected:
  bool read_config();
  void register_signals();
  bool start_services();
  bool stop_services();

 private:
  class Data;
  Services services_;
  std::unique_ptr<Data> d_;
};

/* ========================================================================== */
};  // namespace new_daemon
/* ========================================================================== */
