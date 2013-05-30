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
  public:
    GpuService(const std::string & name, ISvcLocator * sl);
    virtual ~GpuService();

    // IGpuService implementation
    virtual void searchByPair(
      const PixelEvent      & event,
      std::vector<GpuTrack> & tracks);

    // Service implementation
    virtual StatusCode queryInterface(const InterfaceID & riid, void ** ppvIF);
    virtual StatusCode initialize();
    virtual StatusCode finalize();

  private:
    void cleanup();
    void initIO();

  private:
    ITransport * m_transport;
    IProtocol  * m_protocol;
    Tracker    * m_tracker;

    StringProperty m_socketPath;
};
