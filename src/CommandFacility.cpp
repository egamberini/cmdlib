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
  if (m_active.load()) {
    m_active.store(false);
    if(m_executor.joinable()) {
      m_executor.join();
    } 
  }
}

void 
CommandFacility::set_commanded(CommandedObject& commanded) 
{
  if (m_commanded_object == nullptr) {
    m_commanded_object = &commanded;
    m_command_callback = std::bind(&CommandFacility::handle_command, this, std::placeholders::_1);
    m_active.store(true);
    m_executor = std::thread(&CommandFacility::executor, this);
  } else {
    ers::error(CommandFacilityError(ERS_HERE, "set_commanded should be called once."));
  }
}

void 
CommandFacility::execute_command(const cmdobj_t& command)
{
  auto execfut = std::async(std::launch::deferred, m_command_callback, std::move(command));
  m_completion_queue.push(std::move(execfut));
}

void
CommandFacility::handle_command(const cmdobj_t& command) 
{
  std::string ret = "";
  try {
    m_commanded_object->execute(command);
    ret = "OK";
  } catch (const ers::Issue& ei ) {
    ret = ei.what();
    ers::error(CommandedObjectExecutionError(ERS_HERE, "Caught ers::Issue", ei));
  } catch (const std::exception& exc) {
    ret = exc.what();
    ers::error(CommandedObjectExecutionError(ERS_HERE, "Caught std::exception", exc));
  } catch (...) {  // NOLINT JCF Jan-27-2021 violates letter of the law but not the spirit
    ret = "Caught unknown exception";
    ers::error(CommandedObjectExecutionError(ERS_HERE, ret));
  }
  completion_callback(ret);
}

void
CommandFacility::executor()
{
  std::future<void> fut; 
  while (m_active.load()) {
    if (m_completion_queue.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } else {
      bool success = m_completion_queue.try_pop(fut);
      if (!success) {
        ers::error(CommandFacilityError(ERS_HERE, "Can't get from completion queue.")); 
      } else {
        fut.wait(); // trigger execution
      }  
    }
  }
}
