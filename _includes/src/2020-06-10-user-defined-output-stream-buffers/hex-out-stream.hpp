// hex-out-stream.hpp

#pragma once

#include <ostream>

#include "hex-out-stream-nobuf-improved.hpp"

class HexOStream : public std::ostream {
public:
    template<typename... Args>
    HexOStream(Args &&...args) noexcept :
        std::ostream(nullptr), m_buf(std::forward<Args>(args)...) {
        if (m_buf.IsOpen()) {
            rdbuf(&m_buf);
        } else {
            setstate(ios_base::failbit);
        }
    }

private:
    HexOutBuf m_buf;
};
