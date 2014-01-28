#pragma once

#include <exception>
#include <string>

/// Serious exception, recording errno.
class SystemException : public std::runtime_error {
  public:
    /// Construct an instance of SystemException with the given message.
    explicit SystemException(const std::string & message);
    virtual ~SystemException() throw();

    /// Get string identifying exception.
    virtual const char * what() const throw();

  private:
    int         m_errno;
    std::string m_message;
};
