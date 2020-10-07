#include "cmdlib/CommandFacility.hpp"
#include "ers/ers.h"

#include <cetlib/BasicPluginFactory.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>

using namespace dunedaq::cmdlib;
using namespace std::chrono_literals;

class dummyCommandFacility : public CommandFacility
{
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
          inherited::executeCommand(std::to_string(i));
          inherited::executeCommand(badcmd);
        }
        once = false;
      }
    }
    ERS_INFO("Finished.");
  }

protected:
  typedef CommandFacility inherited;

  void completionCallback(const std::string& result) {
    ERS_INFO("Dummy handler just prints out result of cmd: " << result);
  }

};

extern "C" {
  std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
    return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new dummyCommandFacility(uri));
  }
}
