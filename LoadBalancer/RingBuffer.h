#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

template<class T>
class RingBuffer
{
public:
    RingBuffer()
        : m_first(0)
        , m_size()
    {}

    RingBuffer(size_t maxSize)
        : RingBuffer()
    {
        resize(maxSize);
    }

    void append(const T& t)
    {
        size_t i = (m_first + m_size) % m_vector.size();
        m_vector[i] = t;

        if (m_size == m_vector.size()) {
            m_first = (m_first + 1) % m_vector.size();
        }
        else {
            m_size++;
        }
    }

    size_t size() const { return m_size; }

    T& operator[](size_t i)
    {
        assert(i < m_size);
        return m_vector[(m_first + i) % m_vector.size()];
    }

    const T& operator[](size_t i) const
    {
        return const_cast<RingBuffer*>(this)->operator[](i);
    }

    void resize(size_t maxSize)
    {
        assert(m_size == 0);
        m_vector.resize(maxSize);
    }

private:
    size_t m_first;
    size_t m_size;
    std::vector<T> m_vector;
};
