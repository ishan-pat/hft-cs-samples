#include "ring_buffer.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    constexpr std::size_t N = 1 << 20;
    rb::RingBuffer<int> q{N};

    std::thread prod([&] {
        for (int i = 0; i < 1'000'000; ++i)
            while (!q.push(i)) ;
    });
    std::thread cons([&] {
        long long sum = 0;
        for (int i = 0; i < 1'000'000; ++i) {
            std::optional<int> v;
            while (!(v = q.pop())) ;
            sum += *v;
        }
        std::cout << "checksum=" << sum << '\n';
    });

    auto t0 = std::chrono::steady_clock::now();
    prod.join(); cons.join();
    auto dt = std::chrono::duration<double>(std::chrono::steady_clock::now()-t0);
    std::cout << "elapsed " << dt.count()*1e3 << " ms\n";
}
