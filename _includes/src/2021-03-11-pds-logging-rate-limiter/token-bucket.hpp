// token-bucket.hpp

#pragma once

#include <memory>

#include "rate.hpp"
#include "token.hpp"

class TokenBucketLimiter {
public:
    explicit TokenBucketLimiter(Rate rate) :
        m_rate(std::move(rate)), m_token_count(std::make_shared<long>(0)) {
        *m_token_count = m_capacity = m_rate.CountPerSecond();
    }

    TokenBucketLimiter(Rate rate, const long capacity) :
        m_rate(std::move(rate)), m_capacity(capacity),
        m_token_count(std::make_shared<long>(capacity)) {
    }

    auto FetchToken() {
        fill();

        if (*m_token_count > 0) {
            --(*m_token_count);
            return Token {m_token_count};
        }

        return Token {};
    }

private:
    void fill() {
        const auto now = std::chrono::steady_clock::now();
        const auto duration = now - m_last_time;
        const auto seconds_count =
            std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        if (seconds_count) {
            m_last_time = now;

            *m_token_count =
                std::min(m_capacity, *m_token_count + m_rate.CountPerSecond() * seconds_count);
        }
    }

    std::chrono::time_point<std::chrono::steady_clock> m_last_time =
        std::chrono::steady_clock::now();
    Rate m_rate {0};
    long m_capacity = 0;
    std::shared_ptr<long> m_token_count;
};
