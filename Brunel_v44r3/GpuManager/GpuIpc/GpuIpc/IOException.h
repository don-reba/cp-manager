#pragma once

#include <stdexcept>
#include <string>

/// Non-fatal IO exception, such as connection closing.
class IOException : public std::runtime_error {
  public:
    /// Construct an instance of IOException with the given message.
    explicit IOException(const std::string & message);
};
