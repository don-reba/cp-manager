#include "PerfLog.h"

using namespace std;

PerfLog::PerfLog(const char * filePath) :
    m_filePath (filePath) {
}

void PerfLog::addRecord(
    time_t       timestamp,
    const char * kernelName,
    double       totalSeconds,
    double       kernelSeconds,
    size_t       inputSize,
    size_t       outputSize) {
  prepareStream();

  m_out << timestamp           << '\t';
  m_out << escape(kernelName)  << '\t';
  m_out << totalSeconds        << '\t';
  m_out << kernelSeconds       << '\t';
  m_out << inputSize           << '\t';
  m_out << outputSize          << '\n';
  m_out << flush;
}

string PerfLog::escape(const char * str) {
  string result(str);
  for (size_t i = 0, size = result.size(); i != size; ++i) {
    switch (str[i])
    {
      case '\t':
      case '\n':
        result[i] = ' ';
    }
  }
  return result;
}

void PerfLog::prepareStream() {
  // lazy log file use
  if (!m_out.is_open()) {
    m_out.open(m_filePath.c_str(), ofstream::out | ofstream::app);

    // append header, if file is empty
    if (m_out.tellp() == 0)
      m_out << "time\tname\ttotal\tkernel\tin-size\tout-size\n" << flush;
  }
}
