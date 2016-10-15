#include <cmath>

#include "GaudiKernel/ToolFactory.h"

#include "Event/RawEvent.h"

#include "PrPixelHitManager.h"

using namespace PrPixel;

//-----------------------------------------------------------------------------
// Implementation file for class : PrPixelHitManager
//
// 2012-01-05 : Olivier Callot
// 2014-03-11 : Kurt Rinnert
//-----------------------------------------------------------------------------

DECLARE_TOOL_FACTORY(PrPixelHitManager)

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrPixelHitManager::PrPixelHitManager(const std::string &type,
                                     const std::string &name,
                                     const IInterface *parent)
    : GaudiTool(type, name, parent),
      m_maxClusterSize(4),
      m_trigger(false),
      m_clusterLocation(LHCb::VPClusterLocation::Default) {
  declareInterface<PrPixelHitManager>(this);
}

//=============================================================================
// Destructor
//=============================================================================
PrPixelHitManager::~PrPixelHitManager() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PrPixelHitManager::initialize() {

  StatusCode sc = GaudiTool::initialize();
  if (sc.isFailure()) return sc;
  m_isDebug = msgLevel(MSG::DEBUG);
  if (m_isDebug) debug() << "==> Initialize" << endmsg;
  // Get detector element.
  m_vp = getDet<DeVP>(DeVPLocation::Default);
  // Make sure we are up-to-date on populated VELO stations
  registerCondition((*(m_vp->sensorsBegin()))->geometry(),
                    &PrPixelHitManager::rebuildGeometry);
  // First update
  sc = updMgrSvc()->update(this);
  if (!sc.isSuccess()) {
    return Error("Failed to update station structure.");
  }

  // Invalidate measurements at the beginning of each event
  incSvc()->addListener(this, IncidentType::BeginEvent);

  // zero out all buffers
  memset(m_buffer, 0, SENSOR_PIXELS * sizeof(unsigned char));
  memset(m_sp_patterns, 0, 256 * sizeof(unsigned char));
  memset(m_sp_sizes, 0, 256 * sizeof(unsigned char));
  memset(m_sp_fx, 0, 512 * sizeof(float));
  memset(m_sp_fy, 0, 512 * sizeof(float));

  // create pattern buffer for isolated SPs
  cacheSPPatterns();

  // reserve a minimal stack
  m_stack.reserve(64);

  // Setup the hit pool and cluster storage.
  const unsigned int startSize = 10000U;
  m_pool.resize(startSize);
  m_xFractions.resize(startSize);
  m_yFractions.resize(startSize);
  if (!m_trigger) {
    m_channelIDs.resize(startSize);
    m_allHits.resize(startSize);
  }
  m_maxSize = 0;
  m_nHits = 0;
  m_nClusters = 0;
  m_eventReady = false;

  m_nextInPool = m_pool.begin();

  return StatusCode::SUCCESS;
}

//=========================================================================
//  Finalize method.
//=========================================================================
StatusCode PrPixelHitManager::finalize() {
  info() << "Maximum number of Velo hits " << m_maxSize << endmsg;
  return GaudiTool::finalize();
}

//=========================================================================
// Cache Super Pixel cluster patterns.
//=========================================================================
void PrPixelHitManager::cacheSPPatterns() {

  // create a cache for all super pixel cluster patterns.
  // this is an unoptimized 8-way flood fill on the 8 pixels
  // in the super pixel.
  // no point in optimizing as this is called once in
  // initialize() and only takes about 20 us.

  // define deltas to 8-connectivity neighbours
  const int dx[] = {-1, 0, 1, -1, 0, 1, -1, 1};
  const int dy[] = {-1, -1, -1, 1, 1, 1, 0, 0};

  // clustering buffer for isolated superpixels.
  unsigned char sp_buffer[8];

  // SP index buffer and its size for single SP clustering
  unsigned char sp_idx[8];
  unsigned char sp_idx_size = 0;

  // stack and stack pointer for single SP clustering
  unsigned char sp_stack[8];
  unsigned char sp_stack_ptr = 0;

  // loop over all possible SP patterns
  for (unsigned int sp = 0; sp < 256; ++sp) {
    sp_idx_size = 0;
    for (unsigned int shift = 0; shift < 8; ++shift) {
      const unsigned char p = sp & (1 << shift);
      sp_buffer[shift] = p;
      if (p) {
        sp_idx[sp_idx_size++] = shift;
      }
    }

    // loop over pixels in this SP and use them as
    // cluster seeds.
    // note that there are at most two clusters
    // in a single super pixel!
    unsigned char clu_idx = 0;
    for (unsigned int ip = 0; ip < sp_idx_size; ++ip) {
      unsigned char idx = sp_idx[ip];

      if (0 == sp_buffer[idx]) {
        continue;
      }  // pixel is used

      sp_stack_ptr = 0;
      sp_stack[sp_stack_ptr++] = idx;
      sp_buffer[idx] = 0;
      unsigned char x = 0;
      unsigned char y = 0;
      unsigned char n = 0;

      while (sp_stack_ptr) {
        idx = sp_stack[--sp_stack_ptr];
        const unsigned char row = idx % 4;
        const unsigned char col = idx / 4;
        x += col;
        y += row;
        ++n;

        for (unsigned int ni = 0; ni < 8; ++ni) {
          const char ncol = col + dx[ni];
          if (ncol < 0 || ncol > 1) continue;
          const char nrow = row + dy[ni];
          if (nrow < 0 || nrow > 3) continue;
          const unsigned char nidx = (ncol << 2) | nrow;
          if (0 == sp_buffer[nidx]) continue;
          sp_stack[sp_stack_ptr++] = nidx;
          sp_buffer[nidx] = 0;
        }
      }

      const uint32_t cx = x / n;
      const uint32_t cy = y / n;
      const float fx = x / static_cast<float>(n) - cx;
      const float fy = y / static_cast<float>(n) - cy;

      m_sp_sizes[sp] |= n << (4 * clu_idx);

      // store the centroid pixel
      m_sp_patterns[sp] |= ((cx << 2) | cy) << 4 * clu_idx;

      // set the two cluster flag if this is the second cluster
      m_sp_patterns[sp] |= clu_idx << 3;

      // set the pixel fractions
      m_sp_fx[2 * sp + clu_idx] = fx;
      m_sp_fy[2 * sp + clu_idx] = fy;

      // increment cluster count. note that this can only become 0 or 1!
      ++clu_idx;
    }
  }

  if (m_isDebug) {
    debug() << "==== SP Patterns Start ====" << endmsg;
    for (unsigned int i = 0; i < 256; ++i) {
      unsigned char pattern = m_sp_patterns[i];
      debug() << "-- pattern " << i << " --" << endmsg;
      for (int shift = 3; shift >= 0; --shift) {
        debug() << ((i >> shift) & 1) << (i >> (shift + 4) & 1) << endmsg;
      }
      debug() << (pattern & 3) << " " << ((pattern >> 2) & 1);
      if (pattern & 8) {
        debug() << "\t" << ((pattern >> 4) & 3) << " " << ((pattern >> 6) & 1)
                << endmsg;
      } else {
        debug() << "\tn/a" << endmsg;
      }
    }
    debug() << "==== SP Patterns End   ====" << endmsg;
  }
  return;
}

//=========================================================================
// Rebuild the geometry. Needed in case something changes in the Velo during the
// run...
//=========================================================================
StatusCode PrPixelHitManager::rebuildGeometry() {
  std::vector<DeVPSensor *>::const_iterator its = m_vp->sensorsBegin();

  // get pointers to local x coordinates and pitches
  m_local_x = (*its)->xLocal();
  m_x_pitch = (*its)->xPitch();
  m_pixel_size = (*its)->pixelSize(LHCb::VPChannelID(0, 0, 0, 0)).second;

  float ltg_rot_components[9];
  int idx = 0;
  for (; m_vp->sensorsEnd() != its; ++its) {
    // TODO:
    // if (!(*its)->isReadOut()) continue;

    // get the local to global transformation matrix and
    // store it in a flat float array of sixe 12.
    Gaudi::Rotation3D ltg_rot;
    Gaudi::TranslationXYZ ltg_trans;
    (*its)->geometry()->toGlobalMatrix().GetDecomposition(ltg_rot, ltg_trans);
    ltg_rot.GetComponents(ltg_rot_components);
    idx = 16 * (*its)->sensorNumber();
    m_ltg[idx++] = ltg_rot_components[0];
    m_ltg[idx++] = ltg_rot_components[1];
    m_ltg[idx++] = ltg_rot_components[2];
    m_ltg[idx++] = ltg_rot_components[3];
    m_ltg[idx++] = ltg_rot_components[4];
    m_ltg[idx++] = ltg_rot_components[5];
    m_ltg[idx++] = ltg_rot_components[6];
    m_ltg[idx++] = ltg_rot_components[7];
    m_ltg[idx++] = ltg_rot_components[8];
    m_ltg[idx++] = ltg_trans.X();
    m_ltg[idx++] = ltg_trans.Y();
    m_ltg[idx++] = ltg_trans.Z();
  }

  return StatusCode::SUCCESS;
}

//=========================================================================
// Incident handler
//=========================================================================
void PrPixelHitManager::handle(const Incident &incident) {
  if (IncidentType::BeginEvent == incident.type()) {
    this->clearHits();
  }
}

//=========================================================================
// Clear all the hits from a previous event
//=========================================================================
void PrPixelHitManager::clearHits() {
  if (m_nHits > m_maxSize) m_maxSize = m_nHits;
  if (!m_trigger) {
    for (unsigned int i = 0; i < m_nClusters; ++i) {
      m_channelIDs[i].clear();
    }
    m_nClusters = 0;
  }
  m_nHits = 0;
  m_eventReady = false;
}

//=========================================================================
// Build PrPixelHits from Super Pixel or Cluster Raw Bank
//=========================================================================
void PrPixelHitManager::buildHitsFromRawBank() {
  if (m_eventReady) return;
  m_eventReady = true;

  // Retrieve the RawEvent:
  LHCb::RawEvent *rawEvent =
      GaudiTool::getIfExists<LHCb::RawEvent>(LHCb::RawEventLocation::Default);
  if (NULL == rawEvent) {
    rawEvent =
        GaudiTool::getIfExists<LHCb::RawEvent>(LHCb::RawEventLocation::Other);
  }
  if (NULL == rawEvent) {
    return;
  }

  const std::vector<LHCb::RawBank *> &tBanks =
      rawEvent->banks(LHCb::RawBank::VP);
  if (0 == tBanks.size()) {
    return;
  }

  const unsigned int bankVersion = (*tBanks.begin())->version();
  switch (bankVersion) {
    case 1:
      buildHitsFromLCRawBank(tBanks);
      break;
    case 2:
      buildHitsFromSPRawBank(tBanks);
      break;
    default:
      error() << "Unsupported VP Raw Bank version." << endmsg;
      break;
  }
}

//=========================================================================
// Build PrPixelHits from Super Pixel Raw Bank
//=========================================================================
void PrPixelHitManager::buildHitsFromSPRawBank(
    const std::vector<LHCb::RawBank *> &tBanks) {
  // WARNING:
  // This is a rather long function. Please refrain from breaking this
  // up into smaller functions as this will severely impact the
  // timing performance. And yes, this has been measured. Just don't.

  // Assume binary resolution of hit position. This is the weight.
  const float w = std::sqrt(12.0) / (0.055);

  // Loop over VP RawBanks
  std::vector<LHCb::RawBank *>::const_iterator iterBank = tBanks.begin();
  for (; iterBank != tBanks.end(); ++iterBank) {

    const unsigned int sensor = (*iterBank)->sourceID();
    const unsigned int module = sensor / MODULE_SENSORS;
    const float *ltg = m_ltg + 16 * sensor;

    // reset and then fill the super pixel buffer for a sensor
    // memset(m_sp_buffer,0,256*256*3*sizeof(unsigned char));
    // the memset is too slow here. the occupancy is so low that
    // resetting a few elements is *much* faster.
    const unsigned int nrc = m_pixel_idx.size();
    for (unsigned int irc = 0; irc < nrc; ++irc) {
      m_buffer[m_pixel_idx[irc]] = 0;
    }
    m_pixel_idx.clear();

    const uint32_t *bank = (*iterBank)->data();
    const uint32_t nsp = *bank++;

    // make sure the all pools have sufficient size
    // if we target clusters, make sure the space point pool is large enough
    if (m_pool.size() < m_nHits + nsp * 2) {
      const unsigned int newSize = m_nHits + nsp * 2 + 100;
      m_pool.resize(newSize);
      if (m_trigger) {
        m_xFractions.resize(newSize);
        m_yFractions.resize(newSize);
      }
    }
    if (!m_trigger && (m_channelIDs.size() < m_nClusters + nsp * 2)) {
      const unsigned int newSize = m_nClusters + nsp * 2 + 100;
      m_channelIDs.resize(newSize);
      m_xFractions.resize(newSize);
      m_yFractions.resize(newSize);
      m_allHits.resize(newSize);
    }

    for (unsigned int i = 0; i < nsp; ++i) {
      const uint32_t sp_word = *bank++;
      uint8_t sp = sp_word & 0xFFU;

      if (0 == sp) continue;  // protect against zero super pixels.

      const uint32_t sp_addr = (sp_word & 0x007FFF00U) >> 8;
      const uint32_t sp_row = sp_addr & 0x3FU;
      const uint32_t sp_col = (sp_addr >> 6);
      const uint32_t no_sp_neighbours = sp_word & 0x80000000U;

      // if a super pixel is isolated the clustering boils
      // down to a simple pattern look up.
      // don't do this if we run in offline mode where we want to record all
      // contributing channels; in that scenario a few more us are negligible
      // compared to the complication of keeping track of all contributing
      // channel IDs.
      if (m_trigger && no_sp_neighbours) {
        const int sp_size = m_sp_sizes[sp];
        const uint32_t idx = m_sp_patterns[sp];
        const uint32_t chip = sp_col / (CHIP_COLUMNS / 2);

        if ((sp_size & 0x0F) <= m_maxClusterSize) {
          // there is always at least one cluster in the super
          // pixel. look up the pattern and add it.
          const uint32_t row = idx & 0x03U;
          const uint32_t col = (idx >> 2) & 1;
          const uint32_t cx = sp_col * 2 + col;
          const uint32_t cy = sp_row * 4 + row;

          LHCb::VPChannelID cid(sensor, chip, cx % CHIP_COLUMNS, cy);

          const float fx = m_sp_fx[sp * 2];
          const float fy = m_sp_fy[sp * 2];
          const float local_x = m_local_x[cx] + fx * m_x_pitch[cx];
          const float local_y = (cy + 0.5 + fy) * m_pixel_size;

          const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[9];
          const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[10];
          const float gz = ltg[6] * local_x + ltg[7] * local_y + ltg[11];

          m_xFractions[m_nHits] = fx;
          m_yFractions[m_nHits] = fy;
          m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
          m_nHits++;
        }

        // if there is a second cluster for this pattern
        // add it as well.
        if ((idx & 8) && (((sp_size >> 4) & 0x0F) <= m_maxClusterSize)) {
          const uint32_t row = (idx >> 4) & 3;
          const uint32_t col = (idx >> 6) & 1;
          const uint32_t cx = sp_col * 2 + col;
          const uint32_t cy = sp_row * 4 + row;

          LHCb::VPChannelID cid(sensor, chip, cx % CHIP_COLUMNS, cy);

          const float fx = m_sp_fx[sp * 2 + 1];
          const float fy = m_sp_fy[sp * 2 + 1];
          const float local_x = m_local_x[cx] + fx * m_x_pitch[cx];
          const float local_y = (cy + 0.5 + fy) * m_pixel_size;

          const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[9];
          const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[10];
          const float gz = ltg[6] * local_x + ltg[7] * local_y + ltg[11];

          m_xFractions[m_nHits] = fx;
          m_yFractions[m_nHits] = fy;
          m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
          m_nHits++;
        }

        continue;  // move on to next super pixel
      }

      // this one is not isolated or we are targeting clusters; record all
      // pixels.
      for (uint32_t shift = 0; shift < 8; ++shift) {
        const uint8_t pixel = sp & 1;
        if (pixel) {
          const uint32_t row = sp_row * 4 + shift % 4;
          const uint32_t col = sp_col * 2 + shift / 4;
          const uint32_t idx = (col << 8) | row;
          m_buffer[idx] = pixel;
          m_pixel_idx.push_back(idx);
        }
        sp = sp >> 1;
        if (0 == sp) break;
      }
    }  // loop over super pixels in raw bank

    // the sensor buffer is filled, perform the clustering on
    // clusters that span several super pixels.
    const unsigned int nidx = m_pixel_idx.size();
    for (unsigned int irc = 0; irc < nidx; ++irc) {

      const uint32_t idx = m_pixel_idx[irc];
      const uint8_t pixel = m_buffer[idx];

      if (0 == pixel) continue;  // pixel is used in another cluster

      // 8-way row scan optimized seeded flood fill from here.
      m_stack.clear();

      // mark seed as used
      m_buffer[idx] = 0;

      // initialize sums
      unsigned int x = 0;
      unsigned int y = 0;
      unsigned int n = 0;

      // push seed on stack
      m_stack.push_back(idx);

      // as long as the stack is not exhausted:
      // - pop the stack and add popped pixel to cluster
      // - scan the row to left and right, adding set pixels
      //   to the cluster and push set pixels above and below
      //   on the stack (and delete both from the pixel buffer).
      while (!m_stack.empty()) {

        // pop pixel from stack and add it to cluster
        const uint32_t idx = m_stack.back();
        m_stack.pop_back();
        const uint32_t row = idx & 0xFFU;
        const uint32_t col = (idx >> 8) & 0x3FFU;
        x += col;
        y += row;
        ++n;

        if (!m_trigger) {
          const uint32_t chip = col / CHIP_COLUMNS;
          LHCb::VPChannelID cid(sensor, chip, col % CHIP_COLUMNS, row);
          m_channelIDs[m_nClusters].push_back(cid);
        }

        // check up and down
        uint32_t u_idx = idx + 1;
        if (row < SENSOR_ROWS - 1 && m_buffer[u_idx]) {
          m_buffer[u_idx] = 0;
          m_stack.push_back(u_idx);
        }
        uint32_t d_idx = idx - 1;
        if (row > 0 && m_buffer[d_idx]) {
          m_buffer[d_idx] = 0;
          m_stack.push_back(d_idx);
        }

        // scan row to the right
        for (int c = col + 1; c < SENSOR_COLUMNS; ++c) {
          const uint32_t nidx = (c << 8) | row;
          // check up and down
          u_idx = nidx + 1;
          if (row < SENSOR_ROWS - 1 && m_buffer[u_idx]) {
            m_buffer[u_idx] = 0;
            m_stack.push_back(u_idx);
          }
          d_idx = nidx - 1;
          if (row > 0 && m_buffer[d_idx]) {
            m_buffer[d_idx] = 0;
            m_stack.push_back(d_idx);
          }
          // add set pixel to cluster or stop scanning
          if (m_buffer[nidx]) {
            m_buffer[nidx] = 0;
            x += c;
            y += row;
            ++n;
            if (!m_trigger) {
              const uint32_t chip = c / CHIP_COLUMNS;
              LHCb::VPChannelID cid(sensor, chip, c % CHIP_COLUMNS, row);
              m_channelIDs[m_nClusters].push_back(cid);
            }
          } else {
            break;
          }
        }

        // scan row to the left
        for (int c = col - 1; c >= 0; --c) {
          const uint32_t nidx = (c << 8) | row;
          // check up and down
          u_idx = nidx + 1;
          if (row < SENSOR_ROWS - 1 && m_buffer[u_idx]) {
            m_buffer[u_idx] = 0;
            m_stack.push_back(u_idx);
          }
          d_idx = nidx - 1;
          if (row > 0 && m_buffer[d_idx]) {
            m_buffer[d_idx] = 0;
            m_stack.push_back(d_idx);
          }
          // add set pixel to cluster or stop scanning
          if (m_buffer[nidx]) {
            m_buffer[nidx] = 0;
            x += c;
            y += row;
            ++n;
            if (!m_trigger) {
              const uint32_t chip = c / CHIP_COLUMNS;
              LHCb::VPChannelID cid(sensor, chip, c % CHIP_COLUMNS, row);
              m_channelIDs[m_nClusters].push_back(cid);
            }
          } else {
            break;
          }
        }
      }  // while the stack is not empty

      // we are done with this cluster, calculate
      // centroid pixel coordinate and fractions.
      if (m_trigger) {
        // if we are running in the trigger bail out here to
        // save precious micro seconds.
        if (n <= m_maxClusterSize) {
          const unsigned int cx = x / n;
          const unsigned int cy = y / n;
          const float fx = x / static_cast<float>(n) - cx;
          const float fy = y / static_cast<float>(n) - cy;

          m_xFractions[m_nHits] = fx;
          m_yFractions[m_nHits] = fy;

          // store target (3D point for tracking)
          const uint32_t chip = cx / CHIP_COLUMNS;
          LHCb::VPChannelID cid(sensor, chip, cx % CHIP_COLUMNS, cy);

          const float local_x = m_local_x[cx] + fx * m_x_pitch[cx];
          const float local_y = (cy + 0.5 + fy) * m_pixel_size;
          const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[9];
          const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[10];
          const float gz = ltg[6] * local_x + ltg[7] * local_y + ltg[11];
          m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
          m_nHits++;
        }
      } else {  // we are running in offline mode, compute all 3D points
        const unsigned int cx = x / n;
        const unsigned int cy = y / n;
        const float fx = x / static_cast<float>(n) - cx;
        const float fy = y / static_cast<float>(n) - cy;

        m_xFractions[m_nClusters] = fx;
        m_yFractions[m_nClusters] = fy;

        // store target (cluster and 3D point for tracking)
        const uint32_t chip = cx / CHIP_COLUMNS;
        LHCb::VPChannelID cid(sensor, chip, cx % CHIP_COLUMNS, cy);
        const float local_x = m_local_x[cx] + fx * m_x_pitch[cx];
        const float local_y = (cy + 0.5 + fy) * m_pixel_size;
        const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[9];
        const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[10];
        const float gz = ltg[6] * local_x + ltg[7] * local_y + ltg[11];
        m_allHits[m_nClusters++].setHit(cid, gx, gy, gz, w, w, module);
        if (n <= m_maxClusterSize) {  // but do not hand all hits to the tracking
          m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
          m_nHits++;
        }
      }
    }  // loop over all potential seed pixels
  }    // loop over all banks
}

//=========================================================================
//  PrPixelHits from Cluster Raw Bank
//=========================================================================
void PrPixelHitManager::buildHitsFromLCRawBank(
    const std::vector<LHCb::RawBank *> &tBanks) {

  // Assume binary resolution of hit position. This is the weight.
  const float w = std::sqrt(12.0) / (0.055);

  // The unit of the inter pixel fractions.
  const float fractScale = 1.0 / (1 << (YFRACTSHIFT - XFRACTSHIFT));

  // Loop over raw banks.
  const unsigned int nb = tBanks.size();
  for (unsigned int ib = 0; ib < nb; ++ib) {
    const LHCb::RawBank &bank = *tBanks[ib];
    const unsigned int module = bank.sourceID();

    const unsigned int *bank_data = bank.data();
    const unsigned int header = bank_data[0];
    const unsigned int nc = (header & HEADERNCLUMASK) >> HEADERNCLUSHIFT;

    if (m_pool.size() < m_nHits + nc) {
      const unsigned int newSize = m_nHits + nc + 100;
      m_pool.resize(newSize);
      m_xFractions.resize(newSize);
      m_yFractions.resize(newSize);
    }
    if (!m_trigger && (m_channelIDs.size() < m_nHits + nc)) {
      const unsigned int newSize = m_nHits + nc + 100;
      m_channelIDs.resize(newSize);
      m_allHits.resize(newSize);
    }

    for (unsigned int ic = 0; ic < nc; ++ic) {
      const unsigned int cw = bank_data[ic + 1];
      const unsigned int pixel = (cw & PIXELMASK) >> PIXELSHIFT;
      const unsigned int module_chip = pixel & 0x3f0000L >> 16;
      const unsigned int col = pixel & 0xff00L >> 8;
      const unsigned int row = pixel & 0xffL;
      const unsigned int sensor =
          module * MODULE_SENSORS + module_chip / SENSOR_CHIPS;
      const unsigned int sensor_chip = module_chip % SENSOR_CHIPS;
      const float xfract = fractScale * ((cw & XFRACTMASK) >> XFRACTSHIFT);
      const float yfract = fractScale * ((cw & YFRACTMASK) >> YFRACTSHIFT);

      LHCb::VPChannelID cid(sensor, sensor_chip, col, row);

      const unsigned int cy = cid.row();
      const unsigned int cx = cid.col() + CHIP_COLUMNS * sensor_chip;
      const float dx = xfract * m_x_pitch[cx];
      const float dy = yfract * m_pixel_size;
      float local_x = m_local_x[cx] + dx;
      float local_y = (cy + 0.5) * m_pixel_size + dy;
      const float *ltg = m_ltg + sensor * 16;
      const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[9];
      const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[10];
      const float gz = ltg[6] * local_x + ltg[7] * local_y + ltg[11];

      if (!m_trigger) {
        m_channelIDs[m_nHits].push_back(cid);
        m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
        m_nHits++;
      }
      m_xFractions[m_nHits] = xfract;
      m_xFractions[m_nHits] = yfract;
      m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
    }  // loop over clusters in bank
  }    // loop over banks

  // ensure consistent counters if we don't run in the trigger
  if (!m_trigger) {
    m_nClusters = m_nHits;
  }
}

//=========================================================================
// Wrapper for storing clusters (either trigger or offline).
//=========================================================================
void PrPixelHitManager::storeClusters() {

  // if the event is not ready (that is, we are asked to store
  // clusters without running the pixel tracking before), we have
  // to run the clustering/3D point creation first.
  if (!m_eventReady) {
    buildHitsFromRawBank();
  }

  // store the clusters
  if (m_trigger) {
    storeTriggerClusters();
  } else {
    storeOfflineClusters();
  }
}

//=========================================================================
// Store trigger clusters.
//=========================================================================
void PrPixelHitManager::storeTriggerClusters() {

  // Create cluster container on the TES
  LHCb::VPClusters *clusters = new LHCb::VPClusters();
  put(clusters, m_clusterLocation);

  // We are in trigger configuration and can only create clusters for 3D points
  // that were handed to the pixel tracking. The list of contributing channel
  // IDs for each cluster is not available in this scenario. We will make one
  // entry in the list which is the centroid pixel channel ID. It is possible
  // that this refers to a pixel that has not fired. Hence reliable MC matching
  // is not possible in trigger configuration.
  std::vector<LHCb::VPChannelID> pixels(1);
  for (unsigned int ih = 0; ih < m_nHits; ++ih) {
    const PrPixelHit &hit = m_pool[ih];
    const LHCb::VPChannelID cid = hit.id().vpID();
    const float x = hit.x();
    const float y = hit.y();
    const float z = hit.z();

    // It is possible that two clusters have the same centroid
    // channel ID. This is extremely rare but we have to protect
    // against it. First come, first serve.
    // TODO: carefully study whether these rare cases can create
    // a significant systematic. It's very doubtful, but still...
    if (clusters->object(cid)) {
      warning() << "Duplicate VP channel ID in clustering (trigger): " << cid
                << endmsg;
      continue;
    }

    const float fx = m_xFractions[ih];
    const float fy = m_yFractions[ih];
    pixels[0] = cid;
    clusters->insert(
        new LHCb::VPCluster(std::make_pair(fx, fy), x, y, z, pixels), cid);
  }

  return;
}

//=========================================================================
// Store offline (all) clusters.
//=========================================================================
void PrPixelHitManager::storeOfflineClusters() {

  // Create cluster container on the TES
  LHCb::VPClusters *clusters = new LHCb::VPClusters();
  put(clusters, m_clusterLocation);

  // We are in offline configuration. Provided we are not running on old MC
  // we have the full information about contributing pixels. In case of old MC
  // this was not encoded in the raw bank, and we don't have this information.
  // However, MC matching will work consistently in both cases.
  for (unsigned int ic = 0; ic < m_nClusters; ++ic) {
    const PrPixelHit &hit = m_allHits[ic];
    const LHCb::VPChannelID cid = hit.id().vpID();
    const float x = hit.x();
    const float y = hit.y();
    const float z = hit.z();

    // It is possible that two clusters have the same centroid
    // channel ID. This is extremely rare but we have to protect
    // against it. First come, first serve.
    // TODO: carefully study whether these rare cases can create
    // a significant systematic. It's very doubtful, but still...
    if (clusters->object(cid)) {
      warning() << "Duplicate VP channel ID in clustering (offline): " << cid
                << endmsg;
      continue;
    }

    const float fx = m_xFractions[ic];
    const float fy = m_yFractions[ic];
    clusters->insert(
        new LHCb::VPCluster(std::make_pair(fx, fy), x, y, z, m_channelIDs[ic]),
        cid);
  }

  return;
}

//=========================================================================
// Sort hits by X within every module to speed up the track search
//=========================================================================
void PrPixelHitManager::sortByX() {
  if (m_nHits < 1)
    return;

  auto lessByX = [](const PrPixelHit & h1, const PrPixelHit & h2) { return h1.x() < h2.x(); };

  int start = 0;
  for (size_t i = 1; i != m_nHits; ++i) {
    if (m_pool[start].module() == m_pool[i].module())
      continue;

    sort(
        m_pool.begin() + start,
        m_pool.begin() + i,
        lessByX);

    start = i;
  }

  sort(
      m_pool.begin() + start,
      m_pool.begin() + m_nHits,
      lessByX);
}

//=========================================================================
//  Convert the clusters to PrPixelHits
//=========================================================================
void PrPixelHitManager::buildHits() {

  if (m_eventReady) return;
  m_eventReady = true;

  LHCb::VPClusters *clusters = GaudiTool::get<LHCb::VPClusters>(LHCb::VPClusterLocation::Default);

  // If necessary adjust the size of the hit pool.
  if (clusters->size() > m_pool.size())
    m_pool.resize(clusters->size() + 100);

  // Assume binary resolution of hit position. This is the weight.
  const float w = std::sqrt(12.0) / (0.055);

  for (const auto & cluster : *clusters) {
    const unsigned int module = cluster->channelID().module();

    // store 3D point
    LHCb::VPChannelID cid = cluster->channelID();
    const unsigned int sensor_chip = cid.chip();
    const unsigned int sensor = cid.sensor();
    const unsigned int cy = cid.row();
    const unsigned int cx = cid.col() + CHIP_COLUMNS * sensor_chip;
    const float dx = cluster->fraction().first * m_x_pitch[cx];
    const float dy = cluster->fraction().second * m_pixel_size;
    float local_x = m_local_x[cx] + dx;
    float local_y = (cy + 0.5) * m_pixel_size + dy;
    const float *ltg = m_ltg + sensor * 16;
    const float gx = ltg[0] * local_x + ltg[1] * local_y + ltg[9];
    const float gy = ltg[3] * local_x + ltg[4] * local_y + ltg[10];
    const float gz = ltg[6] * local_x + ltg[7] * local_y + ltg[11];

    m_pool[m_nHits].setHit(LHCb::LHCbID(cid), gx, gy, gz, w, w, module);
    m_nHits++;
  }
}
