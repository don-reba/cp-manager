#include "PerfLog.h"

using namespace std;

PerfLog::PerfLog(const char * filePath) :
    m_filePath (filePath) {
}

void PerfLog::addRecord(time_t timestamp, size_t trial, size_t size, double seconds) {
  prepareStream();

  m_out << timestamp << '\t';
  m_out << trial     << '\t';
  m_out << size      << '\t';
  m_out << seconds   << '\n';
  m_out << flush;
}

void PerfLog::prepareStream() {
  // lazy log file use
  if (!m_out.is_open()) {
    m_out.open(m_filePath.c_str(), ofstream::out | ofstream::app);

    // append header, if file is empty
    if (m_out.tellp() == 0)
      m_out << "timestamp\ttrial\tsize\tseconds\n" << flush;
  }
}
