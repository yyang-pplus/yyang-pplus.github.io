#include <array>
#include <cassert>
#include <stdexcept>


class UniqueArray {
    static const int capacity = 1024;
    std::array<int, capacity> m_array;
    int m_last = 0;

public:
    auto Insert(const int v) {
        if (m_last >= capacity) {
            throw std::out_of_range {"UniqueArray failed to insert"};
        }

        m_array[m_last] = v;
        for (int i = 0; true; ++i) {
            if (m_array[i] == v) {
                if (i == m_last) {
                    ++m_last;
                }
                return i;
            }
        }

        assert(false);
    }
};
