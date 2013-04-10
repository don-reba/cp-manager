
#include "TrackerProcessor.h"
#include "TrackerID.h"
#include "FastTrackIpc/IProtocol.h"


TrackerProcessor::TrackerProcessor(){
  _t = Tracker();
}

//-------------------------------
// service functions to implement
//-------------------------------

// service function wrappers
void TrackerProcessor::process_isPrime(IProtocol & protocol) const {
  int32_t n = protocol.readInt32();
  bool result = this->isPrime(n);
  protocol.writeBool(result);
};

void TrackerProcessor::process_factor(IProtocol & protocol) const {
  int32_t n = protocol.readInt32();
  FactorizationMethod method = static_cast<FactorizationMethod>(protocol.readInt32());
  std::vector<int32_t> result = this->factor(n, method);
  protocol.writeInt32(result.size());
  for (int i = 0, size = result.size(); i != size; ++i) {
    protocol.writeInt32(result[i]);
  };
};

void TrackerProcessor::process_searchByPair(IProtocol & protocol) const {
  
  // Receive
  std::vector<char> data(protocol.readInt32());
  
  // char * data_char_pointer = (char*) &data[0];
  // protocol.read(data_char_pointer, data.size());

  for (int i = 0, size = data.size(); i != size; ++i) {
    data[i] = protocol.readChar();
  };
  std::cout << "Received " << data.size() << " bytes" << std::endl;

  // Pocess
  std::vector<char> result = _t.searchByPair(data);

  // Send back
  protocol.writeInt32(result.size());
  
  // protocol.write((char*) &result[0], result.size());
  for (int i = 0, size = result.size(); i != size; ++i) {
    protocol.writeChar(result[i]);
  };
};

// service functions for the user to implement
bool TrackerProcessor::isPrime(int32_t n) const {
  if (n < 2)
    return false;
  for (int i = 2; i != n; ++i) {
    if (n % i == 0)
      return false;
  }
  return true;
};

std::vector<int32_t> TrackerProcessor::factor(int32_t n, FactorizationMethod method) const {
  return std::vector<int32_t>();
};

//---------
// wrappers
//---------

// IProcess implementation
bool TrackerProcessor::process(IProtocol & protocol) const {
  int id = protocol.readInt32();
  switch (id) {
  case TrackerID_isPrime:
    process_isPrime(protocol);
    break;
  case TrackerID_factor:
    process_factor(protocol);
    break;
  case TrackerID_searchByPair:
    process_searchByPair(protocol);
    break;
  default:
    throw std::runtime_error("Tracker::process: unknown ID");
  };
  return true;
};

