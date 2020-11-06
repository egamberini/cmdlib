/**
 * @file stdinCommandFacility.cpp CommandFacility implementation
 * that reads commands from std::cin
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"
#include "cmdlib/Issues.hpp"

#include <ers/ers.h>
#include <nlohmann/json.hpp>
#include <cetlib/BasicPluginFactory.h>

#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <fstream>

using namespace dunedaq::cmdlib;
using namespace std::chrono_literals;
using json = nlohmann::json;

class stdinCommandFacility : public CommandFacility
{
public:
  explicit stdinCommandFacility(std::string uri) : CommandFacility(uri) { 
    // Allocate resources as needed
    auto col = uri.find_last_of(':');
    auto sep = uri.find("://");
    if (col == std::string::npos || sep == std::string::npos) { // enforce URI
      throw dunedaq::cmdlib::MalformedUriError(ERS_HERE, "Malformed URI: ", uri);
    }
    std::string scheme = uri.substr(0, sep);
    std::string fname = uri.substr(sep+3);

    ERS_INFO("Loading commands from file: " << fname);
    try {
      std::ifstream ifs(fname);
      raw_commands_ = json::parse(ifs);
    } catch (const std::exception& ex) {
      ers::error(dunedaq::cmdlib::CommandParserError(ERS_HERE, ex.what()));
    }
    std::ostringstream avaostr;
    avaostr << "Available commands:";
    for (auto it = raw_commands_.begin(); it != raw_commands_.end(); ++it) {
      std::string idstr(it.value()["id"]);
      available_commands_[idstr] = it.value();
      avaostr << " | " << idstr;
    }
    available_str_ = avaostr.str();
  }

  // Implementation of the runner
  void run(std::atomic<bool>& end_marker) {
    ERS_INFO("Entered commands will be launched on CommandedObject...");
    std::string cmdid;
    while (end_marker) { //until runmarker
      ERS_INFO(available_str_);
      // feed commands from cin
      std::cin >> cmdid;
      if (std::cin.eof()) {
        break;
      }
      if ( available_commands_.find(cmdid) == available_commands_.end() ) {
        ERS_INFO("Command " << cmdid << " is not available...");
      } else {
        ERS_INFO("Executing " << cmdid << " command...");
        inherited::executeCommand(available_commands_[cmdid]);
      }
    }
    ERS_INFO("Command handling stopped.");
  }

protected:
  typedef CommandFacility inherited;

  json raw_commands_;
  std::map<std::string, json> available_commands_;
  std::string available_str_;

  // Implementation of completionHandler interface
  void completionCallback(const std::string& result) {
    ERS_INFO("Command execution resulted with: " << result);
  }

};

extern "C" {
    std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
        return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new stdinCommandFacility(uri));
    }
}
