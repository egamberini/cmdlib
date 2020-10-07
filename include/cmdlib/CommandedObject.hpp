/**
 * @file CommandedObject.hpp CommandedObject interface
 */
#ifndef CMDLIB_INCLUDE_COMMANDEDOBJECT_HPP_
#define CMDLIB_INCLUDE_COMMANDEDOBJECT_HPP_

#include <string>

namespace dune::daq::ccm {

typedef std::string Command;

/**
 * @brief Interface needed by commanded objects in the DAQ
 */
class CommandedObject
{
public:
  virtual void execute(Command) = 0; 
};

}

#endif // CMDLIB_INCLUDE_COMMANDEDOBJECT_HPP_
