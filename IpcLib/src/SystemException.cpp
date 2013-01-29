#include "SystemException.h"

#include <cerrno>
#include <cstring>

using namespace std;

SystemException::SystemException(const char * message) :
    m_usrMessage(message),
    m_sysMessage(strerror(errno)) {
  m_message.reserve(m_usrMessage.size() + 1 + m_sysMessage.size());
  m_message.append(m_usrMessage);
  m_message.append(" ");
  m_message.append(m_sysMessage);
}

SystemException::~SystemException() throw() {
}

const char * SystemException::what() const throw() {
  return m_message.c_str();
}
