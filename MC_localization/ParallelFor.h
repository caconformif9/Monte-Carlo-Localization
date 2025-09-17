#pragma once
#include <future>
#include <vector>
#include <algorithm>

template <class F>
inline void parallel_for(size_t begin, size_t end, size_t grain, F&& body) {
    if (begin >= end) return;
    if (grain == 0) grain = 1;
    std::vector<std::future<void>> futs;
    futs.reserve((end - begin + grain - 1) / grain);
    for (size_t b = begin; b < end; b += grain) {
        size_t e = std::min(b + grain, end);
        futs.emplace_back(std::async(std::launch::async, [=, &body] {
            for (size_t i = b; i < e; ++i) body(i);
            }));
    }
    for (auto& f : futs) f.get();
}
