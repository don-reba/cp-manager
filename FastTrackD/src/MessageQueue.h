#pragma once

#include "App.h"

#include "FastTrackIpc/IProtocol.h"

#include <map>

#include <boost/function.hpp>

/** @class MessageQueue MessageQueue.h FastTrackD/MessageQueue.h
 * Connects to a socket and listens for messages.
 *
 * @author Alexey Badalov
 * @date   2013-01-20
 */
class MessageQueue {
  public: // types
    typedef boost::function<void (IProtocol &)> Handler;

    typedef std::map<std::string, Handler> Handlers;

  private: // data

  public: // interface
    /// @param port identifies the socket channeling the messages.
    MessageQueue(unsigned short port);

    /// Starts listening for messages.
    /// @handlers is a map of message identifiers and handler functions.
    /// @endCommand is the identifier of the last command on a connection.
    /// @terminationCommand terminates the messages queue.
    void run(
        const Handlers & handlers,
        const char     * endCommand,
        const char     * terminationCommand);
};
