#include "App.h"
#include "CommandLine.h"
#include "Controller.h"
#include "Logger.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace boost;
using namespace std;

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

// Main entry point.
int main(int argc, char * argv[])
try {
  CommandLine cl(false, "/tmp/GpuManager");
  if (!cl.parse(argc, argv))
    return EXIT_SUCCESS;

  if (cl.daemonize())
    doDaemonize();

  const bool useStdIO = !cl.daemonize();
  Logger logger(useStdIO);

  string adminPath   = cl.path() + "-admin";
  string trackerPath = cl.path() + "-tracker";

  if (cl.exit()) {
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
