#include <benchmark/benchmark.h>

namespace {
void BM_VariableInside(benchmark::State& state) {
    size_t elements = state.range(0);
    std::vector<float> x;
    generate_n(std::back_inserter(x), elements,
               []{ static int cnt = 0; return cnt++; });
    std::vector<float> y;
    generate_n(std::back_inserter(y), elements,
               []{ static int cnt = 0; return cnt++; });

    for(auto _: state) {
        for(size_t i = 0; i < elements; ++i) {
            float a = x[i];
            float b = y[i];
            x[i] = a + b;
            y[i] = a - b;
        }
    }
}
BENCHMARK(BM_VariableInside)->RangeMultiplier(2)->Range(2<<16, 2<<20);

void BM_VariableOutside(benchmark::State& state) {
    size_t elements = state.range(0);
    std::vector<float> x;
    generate_n(std::back_inserter(x), elements,
               []{ static int cnt = 0; return cnt++; });
    std::vector<float> y;
    generate_n(std::back_inserter(y), elements,
               []{ static int cnt = 0; return cnt++; });

    for(auto _: state) {
        float a, b;
        for(size_t i = 0; i < elements; ++i) {
            a = x[i];
            b = y[i];
            x[i] = a + b;
            y[i] = a - b;
        }
    }
}
BENCHMARK(BM_VariableOutside)->RangeMultiplier(2)->Range(2<<16, 2<<20);
}
BENCHMARK_MAIN();
