// rate.hpp

#pragma once

#include <cassert>

#include <chrono>

using namespace std::chrono_literals;

class Rate {
public:
    explicit Rate(const long count, const std::chrono::seconds unit = 1s) :
        m_count_per_second(count * 1s / unit) {
    }

    auto CountPerSecond() const {
        return m_count_per_second;
    }

    auto Interval() const {
        assert(m_count_per_second);
        return std::chrono::microseconds(1s) / m_count_per_second;
    }

private:
    long m_count_per_second = 0;
};
