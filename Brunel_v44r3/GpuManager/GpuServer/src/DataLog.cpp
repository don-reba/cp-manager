#include "DataLog.h"

#include <boost/filesystem.hpp>

#include <sstream>
#include <fstream>
#include <stdint.h>

using namespace std;
using namespace boost::filesystem;

void WriteAsChar(ofstream & stream, const void * data, size_t size) {
  stream.write(reinterpret_cast<const char *>(data), size);
}

DataLog::DataLog(bool enabled, const string & dir) :
		m_enabled (enabled),
		m_dir     (dir),
		m_counter (0) {
}

void DataLog::addRecord(const string & handlerName, const Data & data) {
	if (!m_enabled)
		return;

  create_directory(m_dir);

	string   path   (makePath(m_dir, m_counter++));
	ofstream stream (path.c_str(), ios::binary | ios::trunc);
	if (!data.empty() && stream) {
		uint32_t handlerSize = handlerName.size();
    WriteAsChar(stream, &handlerSize, 4);
    WriteAsChar(stream, handlerName.c_str(), handlerSize);

		uint32_t dataSize = data.size();
    WriteAsChar(stream, &dataSize, 4);
    WriteAsChar(stream, &data[0], dataSize);
	}
}

string DataLog::makePath(const string & dir, int counter) {
	ostringstream path;
	path << dir << '/' << counter << ".dat"; // WARN: OS-specific path separator
	return path.str();
}
