#include <benchmark/benchmark.h>

#include <bmcl/Sha3.h>
#include <bmcl/FixedArrayView.h>

#include <vector>

static void setState(uint64_t* state)
{
    for (uint64_t i = 0; i < 25; i++) {
        state[i] = i;
    }
}

static void keccakfBench(benchmark::State& state)
{
    uint64_t hashState[25];
    setState(hashState);
    while (state.KeepRunning()) {
        bmcl::keccakf(hashState);
        benchmark::DoNotOptimize(hashState);
    }
}

template <std::size_t bits, std::size_t size>
void sha3Bench(benchmark::State& state)
{
    std::vector<uint8_t> data;
    data.reserve(size);
    for (std::size_t i = 0; i < size; i++) {
        data.push_back(i);
    }

    while (state.KeepRunning()) {
        bmcl::Sha3<bits> sha3State;
        sha3State.update(data.data(), size);
        sha3State.finalize();
    }
}

BENCHMARK(keccakfBench);

#define SHA3_BENCH(bits)                         \
    BENCHMARK_TEMPLATE2(sha3Bench, bits, 1);     \
    BENCHMARK_TEMPLATE2(sha3Bench, bits, 100);   \
    BENCHMARK_TEMPLATE2(sha3Bench, bits, 1000);  \
    BENCHMARK_TEMPLATE2(sha3Bench, bits, 10000);

SHA3_BENCH(224);
SHA3_BENCH(256);
SHA3_BENCH(384);
SHA3_BENCH(512);

BENCHMARK_MAIN();
