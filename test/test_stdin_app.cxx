/**
 * @file test_stdin_app.cxx Test application for using 
 * the std::cin based CommandFacility implementation
 */

#include "dummy_commanded_object.hpp"
#include "cmdlib/CommandFacility.hpp"

#include <string>

using namespace dunedaq::cmdlib;

int
main(int /*argc*/, char** /*argv[]*/)
{
  std::atomic<bool> run_marker{true};
  DummyCommandedObject obj;
  auto fac = makeCommandFacility(std::string("stdin://"));
  fac->addCommanded(obj);
  fac->run(run_marker);
  return 0;
}
