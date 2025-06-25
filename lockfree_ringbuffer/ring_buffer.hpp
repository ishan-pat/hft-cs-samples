#pragma once
/*
 * Single-Producer / Single-Consumer lock-free ring buffer.
 * Wait-free for both sides. Cache-line padded.
 */
#include <atomic>
#include <cassert>
#include <cstddef>
#include <optional>
#include <vector>

namespace rb {

template <typename T> class RingBuffer {
  public:
    explicit RingBuffer(std::size_t cap)
        : cap_(cap), mask_(cap - 1), buf_(cap) {
        assert((cap & (cap - 1)) == 0 && "capacity must be power of two");
    }

    bool push(const T &v) noexcept {
        auto head = head_.load(std::memory_order_relaxed);
        auto next = (head + 1) & mask_;
        if (next == tail_.load(std::memory_order_acquire)) return false;
        buf_[head] = v;
        head_.store(next, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() noexcept {
        auto tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) return std::nullopt;
        T v = buf_[tail];
        tail_.store((tail + 1) & mask_, std::memory_order_release);
        return v;
    }

  private:
    const std::size_t cap_, mask_;
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
    std::vector<T> buf_;
};

} // namespace rb
