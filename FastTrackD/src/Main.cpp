#include "App.h"

#include "FastTrackIpc/Protocol.h"
#include "FastTrackIpc/SocketServer.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <boost/program_options.hpp>

using namespace std;

namespace po = boost::program_options;

// Run the process as a daemon.
void daemonize() {
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
bool parseCommandLine(int argc, char * argv[], bool & daemonize) {
  po::options_description desc("Supported options");
  desc.add_options()
    ("help",      "display this help message")
    ("daemonize", "run the process as a daemon")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    desc.print(cout);
    return false;
  }

  daemonize = vm.count("daemonize");

  return true;
}

// Main entry point.
int main(int argc, char * argv[])
try {
  bool enableDaemonize(false);
  if (!parseCommandLine(argc, argv, enableDaemonize))
    return EXIT_SUCCESS;

  const bool useStdIO = !enableDaemonize;
  App app(useStdIO);

  if (enableDaemonize)
    daemonize();

  SocketServer server("/tmp/FastTrack");
  Protocol protocol(server);

  server.accept();

  cout << "accepted\n";

  return EXIT_SUCCESS;
} catch (const exception & e) {
  cout << "Unrecoverable error: " << e.what() << '\n';
  syslog(LOG_ERR, "Unrecoverable error: %s", e.what());
  return EXIT_FAILURE;
} catch (...) {
  cout << "Unknown unrecoverable error." << '\n';
  syslog(LOG_ERR, "Unknown unrecoverable error.");
  return EXIT_FAILURE;
}
