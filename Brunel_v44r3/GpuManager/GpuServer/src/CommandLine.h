#include <string>

class CommandLine {
  public:
    CommandLine(bool daemonize, const char * path);

    bool parse(int argc, char * argv[]);

    bool        daemonize() const { return m_daemonize; }
    bool        exit()      const { return m_exit;      }
    std::string path()      const { return m_path;      }

  private:
    bool        m_daemonize;
    bool        m_exit;
    std::string m_path;
};
