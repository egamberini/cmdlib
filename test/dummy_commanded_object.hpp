/**
 * @file dummy_commanded_object_.hpp Dummy commanded object
 * implementation of the CommandedObject interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_TEST_DUMMY_COMMANDED_OBJECT_HPP_
#define CMDLIB_TEST_DUMMY_COMMANDED_OBJECT_HPP_

#include "cmdlib/CommandedObject.hpp"

#include "ers/ers.h"

#include <iostream>
#include <stdexcept>
#include <string>

class DummyCommandedObject : public dunedaq::cmdlib::CommandedObject
{
public:
  void execute(std::string cmd) {
    if (cmd == "asd") {
      ERS_INFO(cmd << " is a REALLY slow command");
      std::this_thread::sleep_for(std::chrono::seconds(5));
      throw std::runtime_error("Testing throw.");
    } else {
      ERS_INFO("I executed cmd: " << cmd);
    }
  }
};

#endif // CMDLIB_TEST_DUMMY_COMMANDED_OBJECT_HPP_
