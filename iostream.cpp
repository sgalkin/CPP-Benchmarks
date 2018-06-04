#include <benchmark/benchmark.h>

#include <sstream>
#include <iostream>

namespace {
void BM_EndLine(benchmark::State& state) {
  std::stringstream s;
  for(auto _: state) {
      s << "abc" << std::endl;
  }
}
BENCHMARK(BM_EndLine);

void BM_NewLine(benchmark::State& state) {
  std::stringstream s;
  for(auto _: state) {
      s << "abc\n";
  }
}
BENCHMARK(BM_NewLine);
}

BENCHMARK_MAIN();
