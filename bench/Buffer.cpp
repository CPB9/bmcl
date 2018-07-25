#include <benchmark/benchmark.h>

#include <bmcl/Buffer.h>

template <std::size_t size, std::size_t iters>
void bufferConstAppend(benchmark::State& state)
{
    while (state.KeepRunning()) {
        uint8_t data[size];
        bmcl::Buffer buf;
        for (std::size_t i = 0; i < iters; i++) {
            buf.write(data, size);
        }
    }
}

template <std::size_t f1, std::size_t f2, std::size_t iters = 10>
void bufferGrowingAppend(benchmark::State& state)
{
    static uint8_t data[10] = {1,2,3,4,5,6,7,8,9,0};
    while (state.KeepRunning()) {
        bmcl::Buffer buf(data, 10);

        for (std::size_t i = 0; i < iters; i++) {
            buf.write(buf.data(), buf.size() * f1 / f2);
        }
    }
}

BENCHMARK_TEMPLATE2(bufferConstAppend, 1, 1000);
BENCHMARK_TEMPLATE2(bufferConstAppend, 10, 1000);
BENCHMARK_TEMPLATE2(bufferConstAppend, 100, 1000);
BENCHMARK_TEMPLATE2(bufferConstAppend, 1000, 1000);

BENCHMARK_TEMPLATE2(bufferGrowingAppend, 4, 5);
BENCHMARK_TEMPLATE2(bufferGrowingAppend, 2, 3);
BENCHMARK_TEMPLATE2(bufferGrowingAppend, 1, 2);
BENCHMARK_TEMPLATE2(bufferGrowingAppend, 1, 3);
BENCHMARK_TEMPLATE2(bufferGrowingAppend, 1, 4);
BENCHMARK_TEMPLATE2(bufferGrowingAppend, 1, 5);

BENCHMARK_MAIN();
