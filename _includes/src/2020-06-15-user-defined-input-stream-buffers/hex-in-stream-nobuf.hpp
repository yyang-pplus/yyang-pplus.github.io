// hex-in-stream-nobuf.hpp

#pragma once

#include <unistd.h>

#include <streambuf>
#include <string>

class HexInBuf : public std::streambuf {
public:
    using char_type = std::streambuf::char_type;
    using int_type = std::streambuf::int_type;
    using traits_type = std::streambuf::traits_type;

    HexInBuf(const int fd = STD_IN_FD) : m_fd(fd) {
    }

protected:
    static constexpr int STD_IN_FD = 0;
    static constexpr int WIDTH = sizeof(char_type) * 2;

    virtual int_type underflow() override {
        const auto c = uflow();
        if (not traits_type::eq_int_type(c, traits_type::eof())) {
            lseek(m_fd, -WIDTH, SEEK_CUR);
        }

        return c;
    }

    virtual int_type uflow() override {
        std::string hex(WIDTH, 0);
        if (read(m_fd, hex.data(), WIDTH) != WIDTH) {
            return traits_type::eof();
        }

        const char_type c = std::stoi(hex, nullptr, 16);

        return traits_type::to_int_type(c);
    }

    virtual int_type pbackfail(int_type c = traits_type::eof()) override {
        if (lseek(m_fd, -WIDTH, SEEK_CUR) == -1) {
            return traits_type::eof();
        }

        if (traits_type::eq_int_type(c, traits_type::eof())) {
            return traits_type::not_eof(c);
        } else if (traits_type::eq_int_type(c, underflow())) {
            return c;
        }
        return traits_type::eof();
    }

private:
    int m_fd = STD_IN_FD;
};
