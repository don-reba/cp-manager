#include "AdminServer.h"
#include "IApp.h"

AdminServer::AdminServer(IApp & app)
    : m_app (app) {
}

void AdminServer::Exit() {
  m_app.exit();
}
