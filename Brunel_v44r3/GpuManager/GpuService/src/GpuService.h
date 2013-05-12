#pragma once

#include "GpuService/IGpuService.h"

#include <GaudiKernel/Service.h>

#include "GpuIpc/Api/Api.h"

class ITransport;
class IProtocol;
class Tracker;

class GpuService :
    public         Service,
    public virtual IGpuService {
  public:
    GpuService(const std::string & name, ISvcLocator * sl);
    virtual ~GpuService();

    // IFastTrackSvc implementation
    virtual std::vector<GpuTrack> searchByPair(const std::vector<char> & tracks);

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
};