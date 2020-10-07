/**
 * @file CommandFacility.cpp CommandFacility implementation
 */
#include "cmdlib/CommandFacility.hpp"
#include "cmdlib/Issues.hpp"

#include <future>
#include <functional>
#include <string>
#include <atomic>
#include <chrono>

using namespace dunedaq::cmdlib;
using namespace std::literals::chrono_literals;

void 
CommandFacility::addCommanded(CommandedObject& commanded) 
{
  if (commanded_object_ == nullptr) {
    commanded_object_ = &commanded;
    command_callback_ = std::bind(&CommandFacility::handleCommand, this, std::placeholders::_1);
    active_.store(true);
    executor_ = std::thread(&CommandFacility::executor, this);
  } else {
    ers::error(CommandFacilityError(ERS_HERE, "addCommandObject should be called once."));
  }
}

void 
CommandFacility::executeCommand(const std::string& command)
{
  auto execfut = std::async(std::launch::deferred, command_callback_, std::move(command));
  completion_queue_.push(std::move(execfut));
}

void
CommandFacility::handleCommand(const std::string& command) 
{
  std::string ret = "";
  try {
    commanded_object_->execute(command);
    ret = "OK";
  }
  catch (const std::runtime_error& re) {
    ret = re.what();
    ers::error(CommandedObjectExecutionError(ERS_HERE, ret)); 
  }
  completionCallback(ret);
}

void
CommandFacility::executor()
{
  std::future<void> fut; 
  while (active_.load()) {
    if (completion_queue_.empty()) {
      std::this_thread::sleep_for(1s);
    } else {
      bool success = completion_queue_.try_pop(fut);
      if (!success) {
        ers::error(CommandFacilityError(ERS_HERE, "THIS IS BAD, need to understand"));
        
      } else {
        fut.wait(); // trigger execution
      }  
    }
  }
}
