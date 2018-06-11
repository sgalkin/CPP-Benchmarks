#include <benchmark/benchmark.h>

#include <unordered_map>
#include <vector>
#include <array>
#include <cstdint>

namespace {

struct Data {
  explicit Data(uint64_t id) : id(id) {}
  std::array<uint8_t, 256> payload;
  uint64_t id;
};

inline std::pair<uint64_t, Data> MapData(uint64_t id) {
  return std::make_pair(id, Data(id));
}

inline Data VectorData(uint64_t id) {
  return Data(id);
}

// Insert
void BM_UnorderedMapInsert(benchmark::State& state) {
  size_t elements = state.range(0);
  for(auto _: state) {
    state.PauseTiming();
    std::unordered_map<int, Data> data;
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i)
      data.insert(MapData(i));
  }
}
BENCHMARK(BM_UnorderedMapInsert)->RangeMultiplier(2)->Range(2<<3, 2<<10);

void BM_SortedVectorInsert(benchmark::State& state) {
  size_t elements = state.range(0);
  auto cmp = [](const Data& lhs, const Data& rhs) { return lhs.id < rhs.id; };
  for(auto _: state) {
    state.PauseTiming();
    std::vector<Data> data;
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i) {
      auto d = VectorData(i);
      data.insert(std::lower_bound(begin(data), end(data), d, cmp), std::move(d));
    }
  }
}
BENCHMARK(BM_SortedVectorInsert)->RangeMultiplier(2)->Range(2<<3, 2<<10);

void BM_PushBackAndSort(benchmark::State& state) {
  size_t elements = state.range(0);
  auto cmp = [](const Data& lhs, const Data& rhs) { return lhs.id < rhs.id; };
  for(auto _: state) {
    state.PauseTiming();
    std::vector<Data> data;
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i) {
      data.emplace_back(i);
      std::sort(begin(data), end(data), cmp);
    }
  }
}
BENCHMARK(BM_PushBackAndSort)->RangeMultiplier(2)->Range(2<<3, 2<<10);


// Delete
void BM_UnorderedMapDelete(benchmark::State& state) {
  size_t elements = state.range(0);
  for(auto _: state) {
    state.PauseTiming();
    std::unordered_map<int, Data> data;
    for (size_t i = 0; i < elements; ++i)
      data.insert(MapData(i));
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i)
      data.erase(i);
  }
}
BENCHMARK(BM_UnorderedMapDelete)->RangeMultiplier(2)->Range(2<<3, 2<<10);

void BM_SortedVectorDelete(benchmark::State& state) {
  size_t elements = state.range(0);
  auto cmp = [](const Data& lhs, const Data& rhs) { return lhs.id < rhs.id; };
  for(auto _: state) {
    state.PauseTiming();
    std::vector<Data> data;
    for (size_t i = 0; i < elements; ++i) {
      auto d = VectorData(i);
      data.insert(std::lower_bound(begin(data), end(data), d, cmp), std::move(d));
    }
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i) {
      Data d(i);
      auto it = std::lower_bound(begin(data), end(data), d, cmp);
      if(it != end(data) && it->id == i + 1)
        data.erase(it);
    }
  }
}
BENCHMARK(BM_SortedVectorDelete)->RangeMultiplier(2)->Range(2<<3, 2<<10);


// Lookup
void BM_UnorderedMapLookup(benchmark::State& state) {
  size_t elements = state.range(0);
  const Data* d = nullptr;
  for(auto _: state) {
    state.PauseTiming();
    std::unordered_map<int, Data> data;
    for (size_t i = 0; i < elements; ++i)
      data.insert(MapData(i));
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i)
      d = &data.at(i);
  }
  d += sizeof(d);
}
BENCHMARK(BM_UnorderedMapLookup)->RangeMultiplier(2)->Range(2<<3, 2<<10);

void BM_SortedVectorLookup(benchmark::State& state) {
  size_t elements = state.range(0);
  auto cmp = [](const Data& lhs, const Data& rhs) { return lhs.id < rhs.id; };
  const Data* d = nullptr;
  for(auto _: state) {
    state.PauseTiming();
    std::vector<Data> data;
    for (size_t i = 0; i < elements; ++i) {
      auto x = VectorData(i);
      data.insert(std::lower_bound(begin(data), end(data), x, cmp), std::move(x));
    }
    state.ResumeTiming();

    for (size_t i = 0; i < elements; ++i) {
      Data l(i);
      auto it = std::lower_bound(begin(data), end(data), l, cmp);
      if(it != end(data) && it->id == i + 1)
        d = &(*it);
    }
    d += sizeof(d);
  }
}
BENCHMARK(BM_SortedVectorLookup)->RangeMultiplier(2)->Range(2<<3, 2<<10);

}

BENCHMARK_MAIN();
