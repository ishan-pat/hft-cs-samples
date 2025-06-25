# hft-cs-samples

**Showcase repo** demonstrating systems‑oriented CS skills for HFT / quant roles.
Three micro‑projects in three languages:

| Dir | Language | What it shows |
|-----|----------|---------------|
| `lockfree_ringbuffer` | C++17 | Single‑producer/single‑consumer lock‑free queue |
| `orderbook_engine` | Rust 1.77 | Minimal limit‑order book with unit tests |
| `udp_feed_handler` | Go 1.22 | Non‑blocking multicast/UDP feed decoder |

Each sub‑dir is self‑contained and <200 LOC, but touches concurrency,
low‑latency data structures, and network I/O.

## Quick Start

```bash
git clone <repo>
cd hft-cs-samples

# C++
g++ -std=c++17 -O3 lockfree_ringbuffer/test_ring_buffer.cpp -o rb && ./rb

# Rust
cd orderbook_engine && cargo test --release && cd ..

# Go
go run udp_feed_handler/main.go   # listens on :9000
```

All code is MIT‑licensed — feel free to reuse.
