#pragma once

#include "Data.h"

#include <string>

class DataLog {
	public: // interface

		DataLog(bool enabled, const std::string & dir);

    // Add a data record.
    // Each record is saved in separate file, sequentially numbered.
    // The file has the following binary format:
    // - uint32: handler name length
    // - utf-8 handler name
    // - uint32: input data size
    // - input data
		void addRecord(const std::string & handlerName, const Data & data);

	private: // private functions

		static std::string makePath(const std::string & dir, int counter);

	private: // data

		bool        m_enabled;
		std::string m_dir;
		int         m_counter;
};
