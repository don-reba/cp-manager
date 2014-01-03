#pragma once

#include <string>

class CommandLine {
  public:
    CommandLine(const char * path);

    bool parse(int argc, char * argv[]);

    bool daemonize() const { return m_daemonize; }
    bool exit()      const { return m_exit;      }

    const std::string & dataDir() const { return m_dataDir; }
    const std::string & path()    const { return m_path;    }

  private:
    bool        m_daemonize;
    bool        m_exit;
    std::string m_dataDir;
    std::string m_path;
};
