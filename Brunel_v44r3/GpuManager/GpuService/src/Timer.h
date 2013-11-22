#pragma once

#include <time.h>

class Timer {
  public:
    Timer(bool enabled);

    void start();
    void stop();

    double secondsElapsed() const;

  private:
    static double toSeconds(const timespec & tp);

  private:
    bool      m_enabled;
    clockid_t m_clock;
    timespec  m_s;
    timespec  m_f;
};
