#include "CommandLine.h"
#include "DataSender.h"
#include "PerfLog.h"

#include "GpuIpc/IProtocol.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

using namespace boost;
using namespace boost::filesystem;
using namespace std;

// Load data input records from the given paths and send them using the given thread count.
void sendData(
    const char * servicePath,
    int          threadCount,
    PerfLog    & perfLog) {
  const size_t pointCount = 200;
  const size_t trialCount = 100;

  default_random_engine rand(0);

  for (size_t trial = 0; trial != trialCount; ++trial) {
    DataSender::Data data;
    for (size_t i = 0; i <= pointCount; ++i)
      data.push_back({ trial, (2 << 20) * i / pointCount });
    shuffle(data.begin(), data.end(), rand);

    mutex m;
    thread_group group;
    for (int i = 0; i != threadCount; ++i)
      group.create_thread(DataSender(i, servicePath, data, m, perfLog));
    group.join_all();
  }
}

int main(int argc, char * argv[])
try {
  const char * defaultServicePath = "/tmp/GpuManager";

  CommandLine cl(defaultServicePath);
  if (!cl.parse(argc, argv))
    return EXIT_SUCCESS;

  PerfLog perfLog("drv-perf.log");

  string socketPath = cl.servicePath();

  sendData(socketPath.c_str(), cl.threadCount(), perfLog);

  return EXIT_SUCCESS;
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << endl;
  return EXIT_FAILURE;
} catch (...) {
  cout << "Unkonwn unrecoverable error." << endl;
  return EXIT_FAILURE;
}
