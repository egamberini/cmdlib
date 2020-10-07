/**
 * @file CommandFacility.hpp CommandFacility interface
 */
#ifndef CMDLIB_INCLUDE_COMMANDFACILITY_HPP_
#define CMDLIB_INCLUDE_COMMANDFACILITY_HPP_

#include "CommandedObject.hpp"

#include <cetlib/BasicPluginFactory.h>
#include <cetlib/compiler_macros.h>

#include <tbb/concurrent_queue.h>

#include <future>
#include <functional>
#include <string>
#include <atomic>

#ifndef EXTERN_C_FUNC_DECLARE_START
#define EXTERN_C_FUNC_DECLARE_START                                                                                   \
  extern "C"                                                                                                          \
  {
#endif

/**
 * @brief Declare the function that will be called by the plugin loader
 * @param klass Class to be defined as a DUNE Command Facility
 */
#define DEFINE_DUNE_COMMAND_FACILITY(klass)                                                                           \
  EXTERN_C_FUNC_DECLARE_START                                                                                         \
  std::unique_ptr<dune::daq::ccm::CommandFacility> make()                                                             \
  {                                                                                                                   \
    return std::unique_ptr<dune::daq::ccm::CommandFacility>(new klass());                                             \
  }                                                                                                                   \
  }

namespace dune::daq::ccm {

/**
 * @brief Interface needed by DAQ apps and services for command handling
 */
class CommandFacility
{
public:
  CommandFacility(std::string /*uri*/) {}
  ~CommandFacility() { active_.store(false); }
  CommandFacility(const CommandFacility&) = 
    delete; ///< CommandFacility is not copy-constructible
  CommandFacility& operator=(const CommandFacility&) =
    delete; ///< CommandFacility is not copy-assignable
  CommandFacility(CommandFacility&&) =
    delete; ///< CommandFacility is not move-constructible
  CommandFacility& operator=(CommandFacility&&) =
    delete; ///< CommandFacility is not move-assignable

  // Meant to be called once from main
  void addCommanded(CommandedObject& commanded);
  // Meant to be called once from main (implementation specific)
  virtual void run(std::atomic<bool>& end_marker) = 0;


protected:
  // Must be implemented to handling the results of the commands
  virtual void completionHandler(const std::string& result) = 0; 

  // Feed commands from the implementation.
  void launchCommand(const std::string& command);


private:
  // Commaned Object to run execute with received commands as parameters
  mutable CommandedObject* commanded_object_ = nullptr;

  // Completion queue for reqistered tasks
  typedef tbb::concurrent_queue<std::future<void>> CompletionQueue;
  CompletionQueue completion_queue_;

  // Request callback function signature
  typedef std::function<void(const std::string&)> CommandCallback;
  CommandCallback command_callback_ = nullptr;

  //  The glue between commanded and completion callback
  void handleCommand(const std::string& command);

  // Single thrad is responsible to trigger tasks 
  std::atomic<bool> active_;
  void executor();
  std::thread executor_;

};

std::shared_ptr<CommandFacility>
makeCommandFacility(std::string const& uri)
{
    auto sep = uri.find("://");
    std::string scheme;
    if (sep == std::string::npos) { // simple path
        scheme = "file";
    }
    else {                  // with scheme
        scheme = uri.substr(0, sep);
    }
    std::string plugin_name = scheme + "CommandFacility";
    static cet::BasicPluginFactory bpf("duneCommandFacility", "make");
    return bpf.makePlugin<std::shared_ptr<CommandFacility>>(plugin_name, uri);
}

}

#endif // CMDLIB_INCLUDE_COMMANDFACILITY_HPP_
