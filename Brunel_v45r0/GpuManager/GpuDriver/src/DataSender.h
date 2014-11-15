#pragma once

#include "PerfLog.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class ITransport;
class IProtocol;

class DataSender {
  private:

    typedef boost::mutex::scoped_lock scoped_lock;

  public:

    struct Item
    {
      size_t trial;
      size_t size;
    };
    typedef std::vector<Item> Data;

  public:

    DataSender(
        int            index,
        const char   * servicePath,
        Data         & data,
        boost::mutex & mutex,
        PerfLog      & perfLog);

    void operator() ();

  private:

    int          m_index;
    Data         & m_data;
    boost::mutex & m_mutex;
    PerfLog      & m_perfLog;

    // pointers are used to make sure DataSender could be copied
    std::shared_ptr<ITransport> m_transport;
    std::shared_ptr<IProtocol>  m_protocol;
};
