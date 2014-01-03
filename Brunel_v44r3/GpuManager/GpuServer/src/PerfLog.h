#pragma once

#include <fstream>
#include <string>

class PerfLog {
  public:

    PerfLog(const char * filePath);

    void addRecord(
        time_t       timestamp,
        const char * kernelName,
        double       totalSeconds,
        double       kernelSeconds,
        size_t       inputSize,
        size_t       outputSize);

  private:

    static std::string escape(const char * str);
    void prepareStream();

  private:

    std::string   m_filePath;
    std::ofstream m_out;
};
