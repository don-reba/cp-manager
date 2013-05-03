#include "IOException.h"

IOException::IOException(const std::string & message)
    : std::runtime_error(message) {
}
