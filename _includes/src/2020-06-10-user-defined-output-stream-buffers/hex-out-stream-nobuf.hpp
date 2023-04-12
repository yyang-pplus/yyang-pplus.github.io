// hex-out-stream-nobuf.hpp

#pragma once

#include <unistd.h>

#include <streambuf>

#include "str-utils.hpp"

class HexOutBuf : public std::streambuf {
public:
    using char_type = std::streambuf::char_type;
    using int_type = std::streambuf::int_type;
    using traits_type = std::streambuf::traits_type;

protected:
    static constexpr int STD_OUT_FD = 1;
    static constexpr int WIDTH = sizeof(char_type) * 2;

    virtual int_type overflow(int_type c) override {
        if (not traits_type::eq_int_type(c, traits_type::eof())) {
            const auto hex_str = ToHex(c, WIDTH);

            if (write(STD_OUT_FD, hex_str.c_str(), hex_str.size()) == -1) {
                return traits_type::eof();
            }
        }

        return traits_type::not_eof(c);
    }
};
