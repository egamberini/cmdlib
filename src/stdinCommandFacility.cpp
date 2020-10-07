#include "cmdlib/CommandFacility.hpp"
#include <cetlib/BasicPluginFactory.h>

#include "ers/ers.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace dune::daq::ccm;
using namespace std::chrono_literals;

class stdinCommandFacility : public CommandFacility
{
protected:
  typedef CommandFacility inherited;

  // Implementation of completionHandler interface
  void completionHandler(const std::string& result) {
    ERS_INFO("Command execution resulted with: " << result);
  }

public:
  explicit stdinCommandFacility(std::string uri) : CommandFacility(uri) { 
    // Allocate resources as needed
  }

  virtual ~stdinCommandFacility() { 
    // Cleanup resources
  }

  // Implementation of the runner
  void run(std::atomic<bool>& end_marker) {
    ERS_INFO("Entered commands will be launched on CommandedObject...");

    std::string cmd;
    while (end_marker) { //until runmarker
      // feed commands from cin
      std::cin >> cmd;

      // exercice base launch (deferred)
      inherited::launchCommand(cmd);
    }
    ERS_INFO("Command handling stopped.");
  }

};

extern "C" {
    std::shared_ptr<dune::daq::ccm::CommandFacility> make(std::string uri) {
        return std::shared_ptr<dune::daq::ccm::CommandFacility>(new stdinCommandFacility(uri));
    }
}
