#include "CommandLine.h"

#include <iostream>

#include <boost/program_options.hpp>

using namespace boost::program_options;
using namespace std;

CommandLine::CommandLine(const char * path) :
    m_daemonize   (false),
    m_exit        (false),
    m_servicePath (path) {
}

bool CommandLine::parse(int argc, char * argv[]) {
  options_description desc("Supported options");
  desc.add_options()
    ("help", "display this help message")
    ("service",   value(&m_servicePath)->default_value(m_servicePath), "socket path" )
    ("daemonize", bool_switch(&m_daemonize), "run the process as a daemon"    )
    ("exit",      bool_switch(&m_exit),      "stop the server"                )
		("load",      value(&m_handlerToLoad),   "load a handler component"       )
    ("datadir",   value(&m_dataDir),         "save transactions to directory" );

  variables_map vm;
  try {
    store(parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      cout << desc << '\n';
      return false;
    }
    notify(vm);
  } catch (const std::exception & e) {
    cerr << e.what() << endl;
    cout << desc << '\n';
    return false;
  }

  return true;
}
