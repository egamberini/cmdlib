#pragma once

#include "cmdlib/CommandedObject.hpp"

#include "ers/ers.h"

#include <iostream>
#include <stdexcept>

using namespace dune::daq::ccm;
using namespace std::literals::chrono_literals;

class DummyCommandedObject : public CommandedObject
{
public:
  void execute(std::string cmd) {
    if (cmd == "asd") {
      ERS_INFO(cmd << " is a REALLY slow command");
      std::this_thread::sleep_for(5s);
      throw std::runtime_error("Testing throw.");
    } else {
      ERS_INFO("I executed cmd: " << cmd);
    }
  }
};
