// token-bucket-logger.hpp

#pragma once

#include "logpp.hpp"
#include "token-bucket.hpp"

class TokenBucketLogger {
public:
    void Info(const std::string_view message) {
        log(Logpp::Level::info, message);
    }

    void Error(const std::string_view message) {
        log(Logpp::Level::error, message);
    }

    explicit TokenBucketLogger(const long log_per_second = 100) :
        m_limiter(Rate {log_per_second}) {
    }

private:
    void log(const Logpp::Level a_level, const std::string_view message) {
        auto a_token = m_limiter.FetchToken();
        if (a_token) {
            if (not m_logger.Log(a_level, message)) {
                a_token.Return();
            }
        }
    }

    TokenBucketLimiter m_limiter;
    Logpp m_logger;
};
