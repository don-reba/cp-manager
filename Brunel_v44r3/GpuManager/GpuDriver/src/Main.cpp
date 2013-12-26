#include "GpuIpc/SocketClient.h"
#include "GpuIpc/IProtocol.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
try {
  return EXIT_SUCCESS;
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << endl;
  return EXIT_FAILURE;
} catch (...) {
  cout << "Unkonwn unrecoverable error." << endl;
  return EXIT_FAILURE;
}
