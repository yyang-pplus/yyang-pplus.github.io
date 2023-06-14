// token.hpp

#pragma once

#include <memory>

class Token {
    friend class TokenBucketLimiter;
    Token() = default;
    explicit Token(const std::shared_ptr<long> count) : m_count(count), m_valid(true) {
    }
    Token(const Token &) = delete;
    Token &operator=(const Token &) = delete;
    Token(Token &&) = default;
    Token &operator=(Token &&) = default;

public:
    ~Token() {
        if (not m_valid) {
            const auto count = m_count.lock();
            if (count) {
                ++(*count);
            }
        }
    }

    void Return() {
        m_valid = false;
    }

    explicit operator bool() const {
        return m_valid;
    }

private:
    std::weak_ptr<long> m_count;
    bool m_valid = false;
};
