/**
 * @file test_dummy_app.cxx Test application for using the
 * dummyCommandFacility with a DummyCommandedObject.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "dummy_commanded_object.hpp"
#include "cmdlib/CommandFacility.hpp"

#include "ers/ers.h"

#include <string>
#include <chrono>
#include <csignal>

using namespace dunedaq::cmdlib;

// Signal carrier
volatile int global_signal;

// Run marker
std::atomic<bool> run_marker{true};

// SIG handler
static void sigHandler(int signal) {
  ERS_INFO("Signal received: " << signal);
  global_signal = signal;
  run_marker.store(false);
}

// Test application to showcase basic functionality
int
main(int /*argc*/, char** /*argv[]*/)
{
  // Setup signals
  std::signal(SIGKILL, sigHandler);
  std::signal(SIGABRT, sigHandler);
  std::signal(SIGQUIT, sigHandler);

  // Setup facility
  DummyCommandedObject obj;
  auto fac = makeCommandFacility(std::string("dummy://"));
  fac->addCommanded(obj);
  fac->run(run_marker);
  return 0;
}
