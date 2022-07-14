// hex_out_stream_nobuf_improved.hpp

#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <streambuf>

#include "str_utils.hpp"

class HexOutBuf : public std::streambuf {
public:
    using char_type = std::streambuf::char_type;
    using int_type = std::streambuf::int_type;
    using traits_type = std::streambuf::traits_type;

    HexOutBuf(const int fd = INVALID_FD) noexcept : m_fd(fd), m_own(false) {
    }

    HexOutBuf(const char *pathname, const int flags, const mode_t mode = 0) noexcept :
        m_fd(open(pathname, flags, mode)), m_own(true) {
    }

    HexOutBuf(const HexOutBuf &) = delete;
    HexOutBuf &operator=(const HexOutBuf &) = delete;

    HexOutBuf(HexOutBuf &&source) noexcept : m_fd(source.m_fd), m_own(source.m_own) {
        source.m_fd = INVALID_FD;
    }
    HexOutBuf &operator=(HexOutBuf &&source) noexcept {
        if (this != &source) {
            closeFile();
            std::swap(m_fd, source.m_fd);
            std::swap(m_own, source.m_own);
        }

        return *this;
    }

    virtual ~HexOutBuf() noexcept {
        closeFile();
    }

    [[nodiscard]] auto IsOpen() const noexcept {
        return m_fd != INVALID_FD;
    }

protected:
    static constexpr int INVALID_FD = -1;
    static constexpr int WIDTH = sizeof(char_type) * 2;

    void closeFile() noexcept {
        if (IsOpen() and m_own) {
            close(m_fd);
        }
        m_fd = INVALID_FD;
        m_own = false;
    }

    virtual int_type overflow(int_type c) override {
        if (not traits_type::eq_int_type(c, traits_type::eof())) {
            const auto hex_str = ToHex(c, WIDTH);

            if (write(m_fd, hex_str.c_str(), hex_str.size()) == -1) {
                return traits_type::eof();
            }
        }

        return traits_type::not_eof(c);
    }

private:
    int m_fd = INVALID_FD;
    bool m_own = false;
};
