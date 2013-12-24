#pragma once

#include <queue>
#include <stdexcept>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

template <typename T>
class BlockingQueue
{
  private:

    typedef boost::mutex::scoped_lock scoped_lock;

  public:

    BlockingQueue() : m_interrupted(false) {}

    void push(const T & item) {
      scoped_lock lock(m_mutex);

      m_queue.push(item);

      if (m_queue.size() == 1)
      {
        lock.unlock();
        m_condition.notify_one();
      }
    }

    T pop() {
      scoped_lock lock(m_mutex);

      while (m_queue.empty() && !m_interrupted)
        m_condition.wait(lock);
      if (m_interrupted)
        throw std::runtime_error("BlockingQueue was interrupted.");

      T item = m_queue.front();
      m_queue.pop();
      return item;
    }

    void interrupt() {
      {
        scoped_lock lock(m_mutex);
        m_interrupted = true;
      }
      m_condition.notify_all();
    }

  private:

    std::queue<T> m_queue;

    bool m_interrupted;

    boost::condition_variable m_condition;
    boost::mutex              m_mutex;
};
