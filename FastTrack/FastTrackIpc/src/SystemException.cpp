#include "SystemException.h"

#include <cerrno>
#include <cstring>
#include <sstream>

using namespace std;

SystemException::SystemException(const char * message) :
    m_errno (errno) {
  stringstream msg;
  msg << message << " (" << m_errno << ") " << strerror(m_errno);
  m_message = msg.str();
}

SystemException::~SystemException() throw() {
}

const char * SystemException::what() const throw() {
  return m_message.c_str();
}
