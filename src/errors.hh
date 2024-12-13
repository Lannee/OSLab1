#pragma once

#include <stdexcept>

namespace oslab1::shell {

class ExecutionError : public std::runtime_error {
  public:
    ExecutionError(std::string_view msg)
        : std::runtime_error("Execution error: " + std::string(msg)) {}
};

class ParsingError : public std::runtime_error {
  public:
    ParsingError(std::string_view msg)
        : std::runtime_error("Parsing error: " + std::string(msg)) {}
};

}