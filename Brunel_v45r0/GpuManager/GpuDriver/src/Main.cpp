#include "CommandLine.h"
#include "DataSender.h"

#include "GpuIpc/IProtocol.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

using namespace boost;
using namespace boost::filesystem;
using namespace std;

// Sort file paths in reverse order when the file names are numbers.
// The order is reversed, because the collection is treated as a stack.
bool DirectoryCompare(const directory_entry & d1, const directory_entry & d2) {
  string f1 = d1.path().stem().string();
  string f2 = d2.path().stem().string();
  int n1 = atoi(f1.c_str()); // 0 for non-numeric
  int n2 = atoi(f2.c_str()); // 0 for non-numeric
  return n2 < n1; // reverse-sort
}

// Load data input records from the given paths and send them using the given thread count.
void sendData(const char * servicePath, int threadCount, vector<directory_entry> & paths) {
  mutex pathsMutex;
  thread_group group;
  for (int i = 0; i != threadCount; ++i)
    group.create_thread(DataSender(i, servicePath, paths, pathsMutex));
  group.join_all();
}

int main(int argc, char * argv[])
try {
  const char * defaultServicePath = "/tmp/GpuManager";

  CommandLine cl(defaultServicePath);
  if (!cl.parse(argc, argv))
    return EXIT_SUCCESS;

  vector<directory_entry> paths;
  copy(directory_iterator(cl.dataPath()), directory_iterator(), back_inserter(paths));
  if (paths.empty())
    return EXIT_SUCCESS;
  sort(paths.begin(), paths.end(), DirectoryCompare);

  string socketPath = cl.servicePath() + "-tracker";

  sendData(socketPath.c_str(), cl.threadCount(), paths);

  return EXIT_SUCCESS;
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << endl;
  return EXIT_FAILURE;
} catch (...) {
  cout << "Unkonwn unrecoverable error." << endl;
  return EXIT_FAILURE;
}
