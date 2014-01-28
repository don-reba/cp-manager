#include "SystemException.h"

#include <cerrno>
#include <cstring>
#include <sstream>

using namespace std;

SystemException::SystemException(const std::string & message) :
    runtime_error (makeMessage(errno, message)) {
}

SystemException::~SystemException() throw() {
}

string SystemException::makeMessage(int error, const string & message) {
  stringstream msg;
  msg << message << " (" << error << ") " << strerror(error);
  return msg.str();
}
