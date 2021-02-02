/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::cmdlib::cmd.
 */
#ifndef DUNEDAQ_CMDLIB_CMD_STRUCTS_HPP
#define DUNEDAQ_CMDLIB_CMD_STRUCTS_HPP

#include <cstdint>

#include <nlohmann/json.hpp>
#include <string>

namespace dunedaq::cmdlib::cmd {

    // @brief The command name.  FIXME: this should be an enum!
    using CmdId = std::string;

    // @brief An opaque object holding lower layer substructure
    using Data = nlohmann::json;

    // @brief Top-level command object structure
    struct Command {

        // @brief Identify the type of command
        CmdId id = "";

        // @brief Command data object with type-specific structure
        Data data = {};
    };

} // namespace dunedaq::cmdlib::cmd

#endif // DUNEDAQ_CMDLIB_CMD_STRUCTS_HPP