#include <benchmark/benchmark.h>

#include <vector>

namespace {
void BM_EraseRemove(benchmark::State& state) {
  size_t elements = state.range(0);
  for(auto _: state) {
    state.PauseTiming();
    std::vector<int> data;
    int cnt = 0;
    generate_n(std::back_inserter(data), elements, [&cnt]{ return cnt++; });
    state.ResumeTiming();

    data.erase(std::remove_if(std::begin(data), std::end(data), [](int v) { return v%3==0; }),
               std::end(data));
  }
}
BENCHMARK(BM_EraseRemove)->RangeMultiplier(2)->Range(2<<14, 2<<18);

void BM_ReserveEmplace(benchmark::State& state) {
  size_t elements = state.range(0);
  for(auto _: state) {
    state.PauseTiming();
    std::vector<int> data;
    int cnt = 0;
    generate_n(std::back_inserter(data), elements, [&cnt]{ return cnt++; });
    state.ResumeTiming();

    std::vector<int> r;
    r.reserve(data.size() / 3);
    for(size_t i = 0; i < data.size(); ++i) {
        if(data[i] % 3 == 0) {
            r.emplace_back(data[i]);
        }
    }
  }
}
BENCHMARK(BM_ReserveEmplace)->RangeMultiplier(2)->Range(2<<14, 2<<18);
}

BENCHMARK_MAIN();
