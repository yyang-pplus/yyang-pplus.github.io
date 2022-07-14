// hex_out_stream_buffer.hpp

#pragma once

#include <unistd.h>

#include <streambuf>

#include "str_utils.hpp"

class HexOutBuf : public std::streambuf {
public:
    using char_type = std::streambuf::char_type;
    using int_type = std::streambuf::int_type;
    using traits_type = std::streambuf::traits_type;

    HexOutBuf(const int fd = STD_OUT_FD) noexcept : m_fd(fd) {
        static_assert(SIZE % WIDTH == 0);

        std::streambuf::setp(m_buffer.begin(), m_buffer.begin() + SIZE / WIDTH - 1);
    }

    virtual ~HexOutBuf() {
        sync();
    }

protected:
    static constexpr int STD_OUT_FD = 1;
    static constexpr int SIZE = 1024;
    static constexpr int WIDTH = sizeof(char_type) * 2;

    auto flushBuffer() noexcept {
        const auto n = pptr() - pbase();
        for (int i = n * WIDTH - WIDTH; i >= 0; i -= WIDTH) {
            const auto hex_str = ToHex(pbase()[i / 2], WIDTH);
            std::copy(hex_str.cbegin(), hex_str.cend(), pbase() + i);
        }

        if (write(m_fd, pbase(), n * WIDTH) != n * WIDTH) {
            return false;
        }
        pbump(-n);

        return true;
    }

    virtual int_type overflow(int_type c) override {
        if (not traits_type::eq_int_type(c, traits_type::eof())) {
            *pptr() = c;
            pbump(1);
        }

        return flushBuffer() ? traits_type::not_eof(c) : traits_type::eof();
    }

    virtual int sync() override {
        return flushBuffer() ? 0 : -1;
    }

private:
    std::array<char_type, SIZE> m_buffer {};
    int m_fd = STD_OUT_FD;
};
