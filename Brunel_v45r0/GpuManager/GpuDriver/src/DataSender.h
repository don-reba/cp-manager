#pragma once

#include <memory>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class ITransport;
class IProtocol;

class DataSender {
	public:

		struct DiffMessage {
			std::string path;
			std::string message;

			DiffMessage(const std::string & path, const std::string & message)
				: path(path), message(message)
			{}
		};

    typedef std::vector<boost::filesystem::directory_entry> directory_entry_vector;

  private:

    typedef boost::mutex::scoped_lock scoped_lock;

  public:

    DataSender(
        int                        index,
        const char               * servicePath,
        directory_entry_vector   & paths,
				std::vector<DiffMessage> & diffMessages,
        boost::mutex             & pathsMutex,
				bool                       verifyOutput);

    void operator() ();

  private:

		std::string diff(
				const std::vector<uint8_t> & data,
				const std::vector<uint8_t> & reference);

    static void readData(
      const char           * path,
      std::string          & handlerName,
      std::vector<uint8_t> & input,
      std::vector<uint8_t> & output);

  private:

    int                        m_index;
    directory_entry_vector   & m_paths;
		std::vector<DiffMessage> & m_diffMessages;
    boost::mutex             & m_mutex;
		bool                       m_verifyOutput;

    // pointers are used to make sure DataSender could be copied
    std::shared_ptr<ITransport> m_transport;
    std::shared_ptr<IProtocol>  m_protocol;
};
