#pragma once

#include <string>

class CommandLine {
  public:
    CommandLine(const char * path);

    bool parse(int argc, char * argv[]);

    bool daemonize() const { return m_daemonize; }
    bool exit()      const { return m_exit;      }

    const std::string & dataDir()       const { return m_dataDir;       }
    const std::string & handlerToLoad() const { return m_handlerToLoad; }
    const std::string & servicePath()   const { return m_servicePath;   }
    const std::string & hostName()      const { return m_hostName;   }
    const int           hostPort()      const { return m_hostPort;   }
    const std::string & connectorType() const { return m_connectorType;   }

  private:
    bool        m_daemonize;
    bool        m_exit;
    std::string m_dataDir;
    std::string m_handlerToLoad;
    std::string m_hostName;
    int         m_hostPort;
    std::string m_servicePath;
    std::string m_connectorType;
};
