#ifndef CMDLIB_INCLUDE_ISSUES_HPP_
#define CMDLIB_INCLUDE_ISSUES_HPP_

#include "ers/ers.h"
#include <string>

namespace dunedaq {

/**
 * @brief cmdlib specific issues
 * */

    ERS_DECLARE_ISSUE(cmdlib, CommandFacilityError,
                      "CommandFacilityError: " << cmdfacerr,
                      ((std::string)cmdfacerr))

    ERS_DECLARE_ISSUE(cmdlib, CommandedObjectExecutionError,
                      "CommandedObject execution error: " << execerr,
                      ((std::string)execerr)) 

}

#endif
