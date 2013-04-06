#pragma once

#include <stdexcept>
#include <string>

class IOException : public std::runtime_error {
  public:
    explicit IOException(const std::string & message);
};
