#pragma once

#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

// requires:
// void (*T)::Name()

template <typename T>
class BlockingBatchQueue
{
  private:

    typedef boost::mutex::scoped_lock scoped_lock;

  public:

    struct interrupted_error : std::runtime_error {
      interrupted_error() :
          std::runtime_error("BlockingBatchQueue was interrupted.") {
      }
    };

  public:

    BlockingBatchQueue() : m_interrupted(false) {}

    void push(const T & item) {
      scoped_lock lock(m_mutex);

      m_queue.push(item);

      if (m_queue.size() == 1)
      {
        lock.unlock();
        m_condition.notify_one();
      }
    }

    void pop(std::string & name, std::vector<T> & batch) {
      scoped_lock lock(m_mutex);

      while (m_queue.empty() && !m_interrupted)
        m_condition.wait(lock);
      if (m_interrupted)
        throw interrupted_error();

			name = m_queue.front()->Name();
      batch.push_back(m_queue.front());
      m_queue.pop();
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
