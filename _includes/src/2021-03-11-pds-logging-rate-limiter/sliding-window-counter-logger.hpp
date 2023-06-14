// sliding-window-counter-logger.hpp

#pragma once

#include <chrono>

#include "logpp.hpp"
#include "rate.hpp"

class SlidingWindowCounterLogger {
public:
    void Info(const std::string_view message) {
        log(Logpp::Level::info, message);
    }

    void Error(const std::string_view message) {
        log(Logpp::Level::error, message);
    }

    explicit SlidingWindowCounterLogger(const long log_per_second = 100) :
        m_rate(log_per_second) {
    }

private:
    void evict(const std::chrono::steady_clock::time_point &floor_now) {
        const auto window_size = 1s;
        const auto diff = floor_now - m_current_window;
        if (diff > 0s) {
            m_current_window = floor_now;
            if (diff == window_size) {
                m_last_count = std::exchange(m_current_count, 0);
            } else {
                assert(diff > window_size);
                m_last_count = m_current_count = 0;
            }
        }
    }

    bool isWithinLimit(const double current_percentage) const {
        const long count = m_last_count * (1 - current_percentage) + m_current_count;
        return count < m_rate.CountPerSecond();
    }

    void insert() {
        ++m_current_count;
    }

    void log(const Logpp::Level a_level, const std::string_view message) {
        const auto now = std::chrono::steady_clock::now();
        const auto floor_now = std::chrono::floor<std::chrono::seconds>(now);

        evict(floor_now);

        const std::chrono::duration<double> diff = now - floor_now;
        const auto window_size = 1s;
        if (isWithinLimit(diff / window_size) and m_logger.Log(a_level, message)) {
            insert();
        }
    }

    Rate m_rate;
    Logpp m_logger;
    std::chrono::steady_clock::time_point m_current_window;
    long m_last_count = 0;
    long m_current_count = 0;
};
