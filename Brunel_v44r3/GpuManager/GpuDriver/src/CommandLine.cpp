#include "CommandLine.h"

#include <iostream>

#include <boost/program_options.hpp>

using namespace boost::program_options;
using namespace std;

CommandLine::CommandLine(const char * servicePath) :
    m_servicePath (servicePath) {
}

bool CommandLine::parse(int argc, char * argv[]) {
  options_description desc("Supported options");
  desc.add_options()
    ("help", "display this help message"                                                                     )
    ("service", value<string>(&m_servicePath)->default_value(m_servicePath), "socket path"                   )
    ("threads", value<int>(&m_threadCount)->default_value(1), "the number of requests to run concurrently"   )
    ("data",    value<string>(&m_dataPath)->required(),       "directory with input gpuserver input records" );

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

  if (m_threadCount <= 0) {
    cout << desc << '\n';
    return false;
  }

  return true;
}
