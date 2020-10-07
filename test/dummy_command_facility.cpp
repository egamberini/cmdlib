#include "cmdlib/CommandFacility.hpp"
#include <cetlib/BasicPluginFactory.h>

#include "ers/ers.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace dune::daq::ccm;
using namespace std::chrono_literals;

class dummyCommandFacility : public CommandFacility
{
protected:
  typedef CommandFacility inherited;

  void completionHandler(const std::string& result) {
    ERS_INFO("Dummy handler just prints out result of cmd: " << result);
  }

public:
  explicit dummyCommandFacility(std::string uri) : CommandFacility(uri) {
  
  }

  virtual ~dummyCommandFacility() { 
  }

  void run(std::atomic<bool>& end_marker) {
    ERS_INFO("Going for a run...");
    std::string badcmd("asd");
    std::string goodcmd("www");

    bool once = true;
    while (end_marker) {
      if (once) {
        for (unsigned int i = 0; i<10; ++i) {
          inherited::launchCommand(std::to_string(i));
          inherited::launchCommand(badcmd);
        }
        once = false;
      }
    }
    ERS_INFO("Finished.");
  }

};

extern "C" {
  std::shared_ptr<dune::daq::ccm::CommandFacility> make(std::string uri) {
    return std::shared_ptr<dune::daq::ccm::CommandFacility>(new dummyCommandFacility(uri));
  }
}
