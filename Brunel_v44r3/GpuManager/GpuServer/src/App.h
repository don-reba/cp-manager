#pragma once

/** @class App App.h GpuServer/App.h
 * Application-wide API.
 *
 * @author Alexey Badalov
 * @date   2013-01-11
 */
class App {
  public:
    /// @param useStdIO switches between the use of the standard streams and
    /// syslog.
    App(bool useStdIO);
    ~App();

    /// Prints an error message.
    /// @param text is the message to be printed.
    void printError   (const char * text);
    //
    /// Prints an information message.
    /// @param text is the message to be printed.
    void printMessage (const char * text);

  private:
    const bool m_useStdIO;
};
