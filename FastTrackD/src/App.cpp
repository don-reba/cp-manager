#include "App.h"

#include <cstdio>
#include <syslog.h>

using namespace std;

App::App(bool useStdIO) :
    m_useStdIO(useStdIO) {
  if (m_useStdIO) {
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);

    openlog("fasttrackd", LOG_CONS, LOG_DAEMON);
  }
}

App::~App() {
  if (m_useStdIO)
    closelog();
}

void App::printError(const char * text) {
  if (m_useStdIO)
    fprintf(stderr, "%s", text);
  else
    syslog(LOG_ERR, "%s", text);
}

void App::printMessage(const char * text) {
  if (m_useStdIO)
    fprintf(stdout, "%s", text);
  else
    syslog(LOG_INFO, "%s", text);
}
