#pragma once

#include <stdexcept>
#include <string>

/// Serious exception, recording errno.
class SystemException : public std::runtime_error {
  public:

    /// Construct an instance of SystemException with the given message.
    explicit SystemException(const std::string & message);

    virtual ~SystemException() throw();

  private:

    static std::string makeMessage(int error, const std::string & message);
};
