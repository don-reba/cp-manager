#pragma once

class IApp {
  public:
    /// Safely terminates the application.
    virtual void exit() = 0;
};
