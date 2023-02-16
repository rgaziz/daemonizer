// daemon.cc 5/03/2021
/* ========================================================================== */
#include "daemon.h"
#include <unistd.h>
#include <csignal>
#include <iostream>

/* ========================================================================== */
using namespace daemonizer;

/* ========================================================================== */
static bool stop_signal = false;

/* ========================================================================== */
void handle_signal(int signal) {
  std::cout << "Signal: {}" << signal << std::endl;
  stop_signal = true;
}

/* ========================================================================== */
bool print_help() {
  std::cerr << "\nStart example:       ./Module -n conf_file_name.conf \n\n";
  std::cerr << "[-h]                  Display this help and exit \n";
  std::cerr << "[-n]                  Start without demonize this program \n";

  return false;
}

/* ========================================================================== */
class Daemonizer::Data {
 public:
  /* ======================================================================== */
  bool parse_arguments(const int argc, const char **argv) {
    int opt = 0;

    while ((opt = ::getopt(argc, const_cast<char **>(argv), "n")) not_eq -1) {
      if (opt == 'n') {
        demonize_ = false;
        continue;
      }

      return print_help();
    }

    if (optind >= argc) {
      return print_help();
    }

    file_path_ = argv[optind];
    return true;
  }

  /* ======================================================================== */
  bool read_settings() noexcept {
    /* Raad config file here */
    return false;
  }

  /* ======================================================================== */
  bool is_demonize() const { return demonize_; }

 private:
  bool demonize_{true};
  std::string file_path_;
};

/* ========================================================================== */
Daemonizer::Daemonizer(const int argc, const char **argv)
    : d_{std::make_unique<Data>()} {
  if (not d_->parse_arguments(argc, argv)) {
    std::exit(EXIT_FAILURE);
  }

  if (not d_->read_settings()) {
    std::cerr << "Read config file failed!" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

/* ========================================================================== */
Daemonizer::~Daemonizer() { stop_services(); }

/* ========================================================================== */
void Daemonizer::add_service(std::shared_ptr<Service> &service) {
  services_.emplace_back(service);
}

/* ========================================================================== */
void Daemonizer::register_signals() {
  std::cout << "Register signals ..." << std::endl;

  struct sigaction sig_action;
  sig_action.sa_handler = handle_signal;
  ::sigemptyset(&sig_action.sa_mask);
  sig_action.sa_flags = SA_RESTART;

  if (::sigaction(SIGINT, &sig_action, nullptr) < 0) {
    std::exit(EXIT_FAILURE);
  }

  if (::sigaction(SIGTERM, &sig_action, nullptr) < 0) {
    std::exit(EXIT_FAILURE);
  }

  if (::sigaction(SIGABRT, &sig_action, nullptr) < 0) {
    std::exit(EXIT_FAILURE);
  }

  std::cout << "Register signals OK" << std::endl;
}

/* ========================================================================== */
bool Daemonizer::start_services() {
  std::cout << "Start services ..." << std::endl;

  if (services_.empty()) {
    return false;
  }

  for (const auto &service : services_) {
    service->start();
  }

  std::cout << "Start services OK" << std::endl;
  return true;
}

/* ========================================================================== */
bool Daemonizer::stop_services() {
  std::cout << "Stop services ..." << std::endl;

  if (services_.empty()) {
    return false;
  }

  for (const auto &service : services_) {
    service->stop();
  }

  std::cout << "Stop services OK" << std::endl;
  return true;
}

/* ========================================================================== */
bool Daemonizer::run() {
  std::cout << "Daemon run" << std::endl;

  if (d_->is_demonize()) {
    if (not daemon(0, 0)) {
      std::cerr << "Demonize failed!" << std::endl;
      return false;
    }
  }

  if (not initialize()) {
    std::cerr << "Initialise failed!" << std::endl;
    return false;
  }

  register_signals();

  if (not start_services()) {
    std::cerr << "Services not started!" << std::endl;
    return false;
  }

  while (not stop_signal) {
    ::sleep(1u);
  }

  if (not stop_services()) {
    std::cerr << "Services not stoped!" << std::endl;
    return false;
  }

  return true;
}

/* ========================================================================== */
