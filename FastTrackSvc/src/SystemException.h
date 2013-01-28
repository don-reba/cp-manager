#pragma once

#include <exception>
#include <string>

class SystemException : public std::exception {
  public:
    explicit SystemException(const char * message);
    virtual ~SystemException() throw();

    virtual const char * what() const throw();

  private:
    std::string m_usrMessage;
    std::string m_sysMessage;
    std::string m_message;
};
