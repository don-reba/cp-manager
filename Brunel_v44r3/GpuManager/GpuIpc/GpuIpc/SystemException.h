#pragma once

#include <exception>
#include <string>

class SystemException : public std::exception {
  public:
    explicit SystemException(const std::string & message);
    virtual ~SystemException() throw();

    virtual const char * what() const throw();

  private:
    int         m_errno;
    std::string m_message;
};
