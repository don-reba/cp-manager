#pragma once

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class ITransport;
class IProtocol;

class DataSender {
  private:

    typedef boost::mutex::scoped_lock scoped_lock;
    typedef std::vector<boost::filesystem::directory_entry> directory_entry_vector;

  public:

    DataSender(
        int                                               index,
        const char                                      * servicePath,
        std::vector<boost::filesystem::directory_entry> & paths,
        boost::mutex                                    & pathsMutex);

    void operator() ();

  private:

    static void readData
      ( const char           * path
      , std::string          & handlerName
      , std::vector<uint8_t> & data
      );

  private:

    int                      m_index;
    directory_entry_vector & m_paths;
    boost::mutex           & m_mutex;

    // pointers are used to make sure DataSender could be copied
    std::shared_ptr<ITransport> m_transport;
    std::shared_ptr<IProtocol>  m_protocol;
};
