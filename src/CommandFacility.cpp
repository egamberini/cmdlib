/**
 * @file CommandFacility.cpp CommandFacility base implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"
#include "cmdlib/Issues.hpp"

#include <future>
#include <functional>
#include <utility>
#include <atomic>
#include <chrono>
#include <string>

using namespace dunedaq::cmdlib;

CommandFacility::~CommandFacility() 
{
  if (active_.load()) {
    active_.store(false);
    if(executor_.joinable()) {
      executor_.join();
    } 
  }
}

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
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } else {
      bool success = completion_queue_.try_pop(fut);
      if (!success) {
        ers::error(CommandFacilityError(ERS_HERE, "Can't get from completion queue.")); 
      } else {
        fut.wait(); // trigger execution
      }  
    }
  }
}
