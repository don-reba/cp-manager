#include "Controller.h"

Controller::Controller(
    Logger     & logger,
    const char * adminPath) :
    // initializers
    m_logger    (logger),
    m_transport (adminPath),
    m_protocol  (m_transport),
    m_admin     (m_protocol) {
}

void Controller::stopServer() {
  m_admin.Exit();
}
