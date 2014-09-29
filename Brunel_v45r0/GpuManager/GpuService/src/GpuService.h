#pragma once

#include "GpuService/IGpuService.h"

#include <GaudiKernel/Property.h>
#include <GaudiKernel/Service.h>

class ITransport;
class IProtocol;
class Tracker;

class GpuService :
    public         Service,
    public virtual IGpuService {
  public: // construction
    GpuService(const std::string & name, ISvcLocator * sl);
    virtual ~GpuService();

  public: // IGpuService implementation
    virtual void submitData(
        std::string  handlerName,
        const void * data,
        const size_t size,
        Alloc        allocResults,
        AllocParam   allocResultsParam);

  public: // Service implementation
    virtual StatusCode queryInterface(const InterfaceID & riid, void ** ppvIF);
    virtual StatusCode initialize();
    virtual StatusCode finalize();

  private:
    void cleanup();
    void initIO();

  private:
    ITransport * m_transport;
    IProtocol  * m_protocol;

    StringProperty  m_socketPath;
    StringProperty  m_serverHost;
    IntegerProperty m_serverPort;
};
