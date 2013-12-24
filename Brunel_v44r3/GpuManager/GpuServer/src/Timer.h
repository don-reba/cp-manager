#pragma once

#include <time.h>

class Timer {
  public:
    Timer();

    void start();
    void stop();

    double secondsElapsed() const;

  private:
    static double toSeconds(const timespec & tp);

  private:
    clockid_t m_clock;
    timespec  m_s;
    timespec  m_f;
};
