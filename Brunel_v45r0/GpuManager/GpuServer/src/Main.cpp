#include "App.h"
#include "DataLog.h"
#include "CommandLine.h"
#include "Controller.h"
#include "PerfLog.h"
#include "Logger.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

//#include <Gaudi/PluginService.h>

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

class PrPixelCudaHandler
{
  public:
    PrPixelCudaHandler();
};

// Main entry point.
int main(int argc, char * argv[])
try {

  /*
  using Gaudi::PluginService::Details::Registry;
  cout << "getting the registry..." << endl;
  const Registry             & registry  = Registry::instance();
  cout << "getting the factories..." << endl;
  const Registry::FactoryMap & factories = registry.factories();

  const char * offset = "    ";

  cout << factories.size() << " plugins found" << endl;
  for (const auto & entry : factories) {
    const string                & name = entry.first;
    const Registry::FactoryInfo & info = entry.second;
    cout << "* " << name << endl;
    if (!info.library.empty())   cout << offset << "library   = " << info.library   << '\n';
    if (!info.type.empty())      cout << offset << "type      = " << info.type      << '\n';
    if (!info.rtype.empty())     cout << offset << "rtype     = " << info.rtype     << '\n';
    if (!info.className.empty()) cout << offset << "className = " << info.className << '\n';
    if (!info.properties.empty()) {
      cout << "    properties:\n";
      for (const auto & entry : info.properties)
        cout << offset << offset << entry.first << ": " << entry.second << '\n';
    }
  }
  */


  const char * defaultPath = "/tmp/GpuManager";

  CommandLine cl(defaultPath);
  if (!cl.parse(argc, argv))
    return EXIT_SUCCESS;

  if (cl.daemonize())
    doDaemonize();

  const bool useStdIO = !cl.daemonize();
  Logger logger(useStdIO);

  PerfLog perfLog("perf.log");

  const bool recordData = !cl.dataDir().empty();
  DataLog dataLog(recordData, cl.dataDir());

  string adminPath   = cl.servicePath() + "-admin";
  string trackerPath = cl.servicePath() + "-tracker";

  if (cl.exit()) {
    Controller controller(logger, adminPath.c_str());
    controller.stopServer();
    return EXIT_SUCCESS;
  }

  if (!cl.handlerToLoad().empty()) {
    Controller controller(logger, adminPath.c_str());
    controller.loadHandler(cl.handlerToLoad());
    return EXIT_SUCCESS;
  }

  App app(logger, perfLog, dataLog, adminPath.c_str(), trackerPath.c_str());
  app.run();

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
