// hex-in-stream-buffer.hpp

#pragma once

#include <unistd.h>

#include <array>
#include <charconv>
#include <streambuf>

class HexInBuf : public std::streambuf {
public:
    using char_type = std::streambuf::char_type;
    using int_type = std::streambuf::int_type;
    using traits_type = std::streambuf::traits_type;

    HexInBuf(const int fd = STD_IN_FD) noexcept : m_fd(fd) {
        setg(m_buffer.begin(), m_buffer.begin(), m_buffer.begin());
    }

    virtual ~HexInBuf() {
        sync();
    }

protected:
    static constexpr int STD_IN_FD = 0;
    static constexpr int WIDTH = sizeof(char_type) * 2;
    static constexpr int SIZE = 512;
    static constexpr int MAX_PUTBACK = 8;

    virtual int_type underflow() override {
        if (gptr() < egptr()) {
            return traits_type::to_int_type(*gptr());
        }

        const auto num_putback = std::min(MAX_PUTBACK, static_cast<int>(gptr() - eback()));
        std::copy(gptr() - num_putback, gptr(), m_buffer.begin());

        auto *const new_gptr = m_buffer.begin() + num_putback;
        const auto n = read(m_fd, new_gptr, (SIZE - num_putback) * WIDTH) / WIDTH * WIDTH;
        if (n <= 0) {
            return traits_type::eof();
        }
        for (int i = 0; i < n; i += WIDTH) {
            std::from_chars(new_gptr + i, new_gptr + i + WIDTH, new_gptr[i / WIDTH], 16);
        }

        setg(m_buffer.begin(), new_gptr, new_gptr + (n / WIDTH));

        return traits_type::to_int_type(*gptr());
    }

    virtual int sync() override {
        if (gptr() < egptr()) {
            if (lseek(m_fd, (egptr() - gptr()) * WIDTH, SEEK_CUR) == -1) {
                return -1;
            }
            setg(eback(), gptr(), gptr());
        }

        return 0;
    }

private:
    std::array<char_type, SIZE * WIDTH> m_buffer;
    int m_fd = STD_IN_FD;
};
