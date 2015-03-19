#include "KernelPreprocessing.h"

#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

using namespace std;

struct KernelPreprocessingFixture {
  Event        event;
  vector<Span> spans;
  const int    candidatesPerThread;
  const int    threadCount;

  KernelPreprocessingFixture() :
      candidatesPerThread (200000),
      threadCount         (32) {
    vector<uint8_t> input;
    readData("../data/0.dat", input);

    parseEvent(input.data(), input.size(), event);

    splitHits(event, threadCount, candidatesPerThread, spans);
  }

  void static readData(const char * path, vector<uint8_t> & input) {
    ifstream stream(path, ios_base::binary);

    //read name
    uint32_t handlerSize;
    readStream(stream, &handlerSize, 4);

    vector<char> handlerChars(handlerSize + 1); // +1 for terminating zero
    readStream(stream, handlerChars.data(), handlerSize);

    // input
    uint32_t inputSize;
    readStream(stream, &inputSize, 4);

    input.resize(inputSize);
    readStream(stream, input.data(), inputSize);
  }

  void static readStream(ifstream  & stream, void * data, size_t size) {
    stream.read(reinterpret_cast<char*>(data), size);
  }
};

// unit tests

BOOST_FIXTURE_TEST_CASE(Test_Count, KernelPreprocessingFixture) {
  BOOST_REQUIRE_EQUAL(spans.size() % threadCount, 0);
}

BOOST_FIXTURE_TEST_CASE(Test_Validity, KernelPreprocessingFixture) {
  for (Span & span : spans) {
    // lexicographic ordering
    BOOST_REQUIRE_LE(span.first.s0, span.last.s0);
    if (span.first.s0 == span.last.s0)
      BOOST_REQUIRE_LE(span.first.s1, span.last.s1);

    // range checks
    const int firstSensor  = span.firstSensor;
    const int secondSensor = firstSensor - 2;

    BOOST_REQUIRE_GE(span.first.s0, 0);
    BOOST_REQUIRE_GE(span.first.s1, 0);

    BOOST_REQUIRE_LE(span.last.s0, event.sensor_hitNums[firstSensor]);
    BOOST_REQUIRE_LE(span.last.s1, event.sensor_hitNums[secondSensor]);
  }
}

BOOST_FIXTURE_TEST_CASE(Test_Structure, KernelPreprocessingFixture) {
  if (spans.empty())
    return;

  BOOST_CHECK_EQUAL(spans.front().first.s0, 0);
  BOOST_CHECK_EQUAL(spans.front().first.s1, 0);

  BOOST_CHECK_EQUAL(spans.back().last.s0, event.sensor_hitNums[4]);
  BOOST_CHECK_EQUAL(spans.back().last.s1, event.sensor_hitNums[2]);

  int  sensor = spans[0].firstSensor;
  auto last   = spans[0].first;
  for (Span & span : spans) {
    BOOST_REQUIRE(span.firstSensor != sensor || span.first == last);
    sensor = span.firstSensor;
    last   = span.last;
  }
}

BOOST_FIXTURE_TEST_CASE(Test_Completeness, KernelPreprocessingFixture) {
  int spanTotal = 0;
  for (Span & span : spans) {
    const int firstSensor  = span.firstSensor;
    const int secondSensor = firstSensor - 2;
    const int lastSensor   = max(firstSensor - 7, 0);

    int i = span.first.s0;
    int j = span.first.s1;

    int count = 0;

    while ((i != span.last.s0 || j != span.last.s1) && i != event.sensor_hitNums[firstSensor]) {
      for (int thirdSensor = firstSensor - 3; thirdSensor != lastSensor; --thirdSensor) {
        spanTotal += event.sensor_hitNums[thirdSensor];
        count += event.sensor_hitNums[thirdSensor];
      }
      ++j;
      if (j == event.sensor_hitNums[secondSensor]) {
        j = 0;
        ++i;
      }
    }
  }

  int total = 0;
  for (int firstSensor = 4; firstSensor < *event.no_sensors; ++firstSensor) {
    const int secondSensor = firstSensor - 2;
    const int lastSensor   = max(firstSensor - 7, 0);
    for (int i = 0; i < event.sensor_hitNums[firstSensor]; ++i) {
      for (int j = 0; j < event.sensor_hitNums[secondSensor]; ++j) {
        for (int thirdSensor = firstSensor - 3; thirdSensor != lastSensor; --thirdSensor)
          total += event.sensor_hitNums[thirdSensor];
      }
    }
  }

  BOOST_REQUIRE_EQUAL(spanTotal, total);
}
