/**
 * @file Issues.hpp cmdlib specific ERS Issues
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_INCLUDE_CMDLIB_ISSUES_HPP_ 
#define CMDLIB_INCLUDE_CMDLIB_ISSUES_HPP_ 

#include <ers/ers.h>
#include <string>

namespace dunedaq {

/**
 * @brief cmdlib specific issues
 * */

    ERS_DECLARE_ISSUE(cmdlib, MalformedUriError,
                      "Malformed uri: " << error << " uri:" << uri,
                      ((std::string)error)
                      ((std::string)uri))

    ERS_DECLARE_ISSUE(cmdlib, CommandParserError,
                      "Failed to parse command: " << error,
                      ((std::string)error))

    ERS_DECLARE_ISSUE(cmdlib, CommandedObjectExecutionError,
                      "CommandedObject execution error: " << error,
                      ((std::string)error)) 

    ERS_DECLARE_ISSUE(cmdlib, CommandFacilityError,
                      "CommandFacilityError: " << error,
                      ((std::string)error))

} // namespace dunedaq  

#endif // CMDLIB_INCLUDE_CMDLIB_ISSUES_HPP_ 
