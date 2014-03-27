#include "Timer.h"

Timer::Timer(bool enabled) :
    m_enabled (enabled),
    m_clock   (CLOCK_REALTIME) {
  m_s.tv_sec  = m_f.tv_sec  = 0;
  m_s.tv_nsec = m_f.tv_nsec = 0;
}

void Timer::start() {
  if (m_enabled)
    clock_gettime(m_clock, &m_s);
}

void Timer::stop() {
  if (m_enabled)
    clock_gettime(m_clock, &m_f);
}

double Timer::secondsElapsed() const {
  return toSeconds(m_f) - toSeconds(m_s);
}

double Timer::toSeconds(const timespec & tp) {
    double  sec = static_cast<double>(tp.tv_sec);
    double nsec = static_cast<double>(tp.tv_nsec);
    return sec + 0.0000000001 * nsec;
}
