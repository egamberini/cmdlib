/**
 * @file daq_application.cxx Main Application for the DAQ Framework, loads
 * DAQModules based on json configuration file
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dummy_commanded_object.hpp"
#include "cmdlib/CommandFacility.hpp"

int
main(int /*argc*/, char** /*argv[]*/)
{
  std::atomic<bool> marker{true};
  DummyCommandedObject cat;
  auto dog = makeCommandFacility(std::string("dummy://"));
  dog->addCommanded(cat);
  dog->run(marker);
  return 0;
}
