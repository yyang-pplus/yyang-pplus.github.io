// hex-in-stream-single-buf.hpp

#pragma once

#include <unistd.h>

#include <streambuf>
#include <string>

class HexInBuf : public std::streambuf {
public:
    using char_type = std::streambuf::char_type;
    using int_type = std::streambuf::int_type;
    using traits_type = std::streambuf::traits_type;

    HexInBuf(const int fd = STD_IN_FD) noexcept : m_fd(fd) {
        setg(&m_buffer, &m_buffer + 1, &m_buffer + 1);
    }

protected:
    static constexpr int STD_IN_FD = 0;
    static constexpr int WIDTH = sizeof(char_type) * 2;

    virtual int_type underflow() override {
        if (gptr() < egptr()) {
            return traits_type::to_int_type(m_buffer);
        }

        std::string hex(WIDTH, 0);
        if (read(m_fd, hex.data(), WIDTH) != WIDTH) {
            return traits_type::eof();
        }

        m_buffer = std::stoi(hex, nullptr, 16);
        gbump(-1);

        return traits_type::to_int_type(m_buffer);
    }

private:
    int m_fd = STD_IN_FD;
    char_type m_buffer {};
};
