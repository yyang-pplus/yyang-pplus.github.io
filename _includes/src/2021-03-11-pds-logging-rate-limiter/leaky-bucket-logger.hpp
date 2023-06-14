// leaky-bucket-logger.hpp

#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "logpp.hpp"
#include "rate.hpp"

class LeakyBucketLogger {
public:
    void Info(std::string message) {
        log(Logpp::Level::info, std::move(message));
    }

    void Error(const std::string message) {
        log(Logpp::Level::error, std::move(message));
    }

    explicit LeakyBucketLogger(const long log_per_second = 100) :
        m_rate(log_per_second), m_capacity(log_per_second) {
    }

    ~LeakyBucketLogger() {
        m_abort = true;
        m_queue_cv.notify_all();
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void Start() {
        if (not m_thread.joinable()) {
            m_thread = std::thread(&LeakyBucketLogger::run, this);
        }
    }

private:
    void log(const Logpp::Level a_level, std::string message) {
        {
            std::lock_guard<std::mutex> guard {m_queue_mutex};
            if (m_queue.size() < m_capacity) {
                m_queue.emplace(a_level, std::move(message));
            }
        }

        m_queue_cv.notify_one();
    }

    auto elapsed() const {
        const auto now = std::chrono::steady_clock::now();
        return now - m_last_time;
    }

    void run() {
        while (true) {
            std::unique_lock<std::mutex> lock {m_queue_mutex};

            m_queue_cv.wait(lock, [this] {
                return m_abort.load() or (not m_queue.empty() and elapsed() >= m_rate.Interval());
            });
            if (m_abort.load()) {
                break;
            }

            const auto &[a_level, message] = m_queue.front();

            if (m_logger.Log(a_level, message)) {
                m_last_time = std::chrono::steady_clock::now();
            }

            m_queue.pop();
        }
    }

    std::thread m_thread;

    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cv;
    std::queue<std::pair<Logpp::Level, std::string>> m_queue;

    Logpp m_logger;

    Rate m_rate {0};
    std::chrono::time_point<std::chrono::steady_clock> m_last_time =
        std::chrono::steady_clock::now();
    std::size_t m_capacity = 0;

    std::atomic<bool> m_abort {false};
};
