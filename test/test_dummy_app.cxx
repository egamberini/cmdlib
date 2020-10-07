/**
 * @file test_dummy_app.cxx Test application for using the
 * dummyCommandFacility with a DummyCommandedObject.
 */

#include "dummy_commanded_object.hpp"
#include "cmdlib/CommandFacility.hpp"

#include <string>

using namespace dunedaq::cmdlib;

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
