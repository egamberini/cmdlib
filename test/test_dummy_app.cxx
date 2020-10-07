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

#include <string>

using namespace dunedaq::cmdlib;

int
main(int /*argc*/, char** /*argv[]*/)
{
  std::atomic<bool> marker{true};
  DummyCommandedObject obj;
  auto fac = makeCommandFacility(std::string("dummy://"));
  fac->addCommanded(obj);
  fac->run(marker);
  return 0;
}
