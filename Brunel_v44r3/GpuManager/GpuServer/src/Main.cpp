#include "App.h"
#include "Controller.h"
#include "Logger.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/program_options.hpp>

using namespace boost;
using namespace std;

namespace po = boost::program_options;

struct Args {
  bool   daemonize;
  bool   exit;
  string path;
};

// Run the process as a daemon.
void doDaemonize() {
  // fork off the parent process
  pid_t pid(fork());
  if (pid > 0)
    exit(EXIT_SUCCESS);
  if (pid < 0)
    throw runtime_error("Fork failed.");

  // change the file mode mask
  umask(0);

  // create a new SID for the child process
  if (setsid() < 0)
    throw runtime_error("Failed to create a new child process SID.");

  // change the current working directory
  if (chdir("/") < 0)
    throw runtime_error("Failed to change the current working directory.");
}

// Parse command line options.
bool parseCommandLine(int argc, char * argv[], /* OUT */ Args & args) {
  args.daemonize = false;
  args.exit      = false;

  po::options_description desc("Supported options");
  desc.add_options()
    ("daemonize",
     po::value<bool>(&args.daemonize)->zero_tokens(),
     "run the process as a daemon")
    ("exit",
     po::value<bool>(&args.exit)->zero_tokens(),
     "stop the server with the given path")
    ("path",
     po::value<string>(&args.path)->default_value("/tmp/GpuManager"),
     "socket path"),
    ("help",
     "display this help message");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      cout << desc << '\n';
      return EXIT_SUCCESS;
    }
    po::notify(vm);
  } catch (const std::exception & e) {
    cerr << e.what() << endl;
    cout << desc << '\n';
    return EXIT_FAILURE;
  }

  return true;
}

// Main entry point.
int main(int argc, char * argv[])
try {
  Args args;
  if (!parseCommandLine(argc, argv, args))
    return EXIT_FAILURE;

  if (args.daemonize)
    doDaemonize();

  const bool useStdIO = !args.daemonize;
  Logger logger(useStdIO);

  string adminPath   = args.path + "-admin";
  string trackerPath = args.path + "-tracker";

  if (args.exit) {
    Controller controller(logger, adminPath.c_str());
    controller.stopServer();
  } else {
    App app(logger, adminPath.c_str(), trackerPath.c_str());
    app.run();
  }

  return EXIT_SUCCESS;
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << '\n';
  syslog(LOG_ERR, "Unrecoverable error: %s", e.what());
  return EXIT_FAILURE;
} catch (...) {
  cout << "Unknown unrecoverable error." << '\n';
  syslog(LOG_ERR, "Unknown unrecoverable error.");
  return EXIT_FAILURE;
}
