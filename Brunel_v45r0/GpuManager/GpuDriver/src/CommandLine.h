#pragma once

#include <string>

class CommandLine {
  public:

    CommandLine(const char * servicePath);

    bool parse(int argc, char * argv[]);

    int         threadCount() const { return m_threadCount; }
    std::string dataPath()    const { return m_dataPath;    }
    std::string servicePath() const { return m_servicePath; }

  private:

    int         m_threadCount;
    std::string m_dataPath;
    std::string m_servicePath;
};
