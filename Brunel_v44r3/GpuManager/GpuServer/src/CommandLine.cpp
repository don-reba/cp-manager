#include "CommandLine.h"

#include <iostream>

#include <boost/program_options.hpp>

using namespace std;

namespace po = boost::program_options;

CommandLine::CommandLine(bool daemonize, const char * path) :
		m_daemonize (daemonize),
		m_exit      (false),
		m_path      (path) {
}

bool CommandLine::parse(int argc, char * argv[]) {
  po::options_description desc("Supported options");
  desc.add_options()
    ("help", "display this help message")
    ("daemonize", po::value<bool>(&m_daemonize)->default_value(m_daemonize), "run the process as a daemon" )
    ("exit",      po::value<bool>(&m_exit)->zero_tokens(),                   "stop the server"             )
    ("path",      po::value<string>(&m_path)->default_value(m_path),         "socket path"                 );

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      cout << desc << '\n';
      return true;
    }
    po::notify(vm);
  } catch (const std::exception & e) {
    cerr << e.what() << endl;
    cout << desc << '\n';
    return false;
  }

	return true;
}
