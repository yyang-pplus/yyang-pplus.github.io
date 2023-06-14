// sliding-log-logger.hpp

#pragma once

#include <chrono>
#include <queue>

#include "logpp.hpp"
#include "rate.hpp"

class SlidingLogLogger {
public:
    void Info(const std::string_view message) {
        log(Logpp::Level::info, message);
    }

    void Error(const std::string_view message) {
        log(Logpp::Level::error, message);
    }

    explicit SlidingLogLogger(const long log_per_second = 100) : m_rate(log_per_second) {
    }

private:
    void evict(const std::chrono::steady_clock::time_point &tp) {
        const auto window_size = 1s;
        while (not m_queue.empty() and (tp - m_queue.front()) > window_size) {
            m_queue.pop();
        }
    }

    void insert(std::chrono::steady_clock::time_point tp) {
        m_queue.push(std::move(tp));
    }

    auto isWithinLimit() const {
        const auto limit = m_rate.CountPerSecond();
        const decltype(limit) size = m_queue.size();
        return size < limit;
    }

    void log(const Logpp::Level a_level, const std::string_view message) {
        auto now = std::chrono::steady_clock::now();

        evict(now);

        if (isWithinLimit() and m_logger.Log(a_level, message)) {
            insert(std::move(now));
        }
    }

    std::queue<std::chrono::steady_clock::time_point> m_queue;
    Rate m_rate;
    Logpp m_logger;
};
