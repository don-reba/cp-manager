#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

// main entry point
// creates a daemon, starts logging, and handles uncaught exceptions
int main(int argc, char * argv[])
try
{
  // fork off the partent process
  pid_t pid(fork());
  if (pid > 0)
  {
    return EXIT_SUCCESS;
  }
  if (pid < 0)
  {
    cerr << "Fork failed." << endl;
    return EXIT_FAILURE;
  }

  // change the file mode mask
  umask(0);

  // create a new SID for the child process
  pid_t sid(setsid());
  if (sid < 0)
  {
    cerr << "Failed to create a new child process SID." << endl;
    return EXIT_FAILURE;
  }

  // change the current working directory
  if (chdir("/") < 0)
  {
    cerr << "Failed to change the current working directory." << endl;
    return EXIT_FAILURE;
  }

  fclose (stdin);
  fclose (stdout);
  fclose (stderr);

  openlog("fasttrackd", LOG_CONS, LOG_DAEMON);

  // run message loop

  closelog();

  return EXIT_SUCCESS;
}
catch (const exception & e)
{
  syslog(LOG_ERR, "Unrecoverable error: %s", e.what());
}
catch (...)
{
  syslog(LOG_ERR, "Unknown unrecoverable error.");
}
