#pragma once

class App {
  public:
    App(bool useStdIO);
    ~App();

    void printError   (const char * text);
    void printMessage (const char * text);

  private:
    const bool m_useStdIO;
};
