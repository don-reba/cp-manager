#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#include <cstdlib>
#include <exception>
#include <stdexcept>

using namespace std;

void daemonize()
{
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

// main entry point
int main(int argc, char * argv[])
try
{
  daemonize();

  freopen("/dev/null", "r", stdin);
  freopen("/dev/null", "w", stdout);
  freopen("/dev/null", "w", stderr);

  openlog("fasttrackd", LOG_CONS, LOG_DAEMON);

  // run message loop

  closelog();

  return EXIT_SUCCESS;
}
catch (const exception & e)
{
  syslog(LOG_ERR, "Unrecoverable error: %s", e.what());
  return EXIT_FAILURE;
}
catch (...)
{
  syslog(LOG_ERR, "Unknown unrecoverable error.");
  return EXIT_FAILURE;
}
