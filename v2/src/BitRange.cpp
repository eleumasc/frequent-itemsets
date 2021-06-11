#include "BitRange.h"

#include <assert.h>

BitRange::BitRange() : m_min(1), m_max(0)
{
}

BitRange::BitRange(int min, int max) : m_min(min), m_max(max)
{
    if (max >= min)
    {
        m_data = std::vector<bool>(max - min + 1);
    }
}

void BitRange::join(const BitRange &other)
{
    if (m_min > m_max)
    {
        return;
    }
    if (other.m_min > other.m_max)
    {
        return;
    }
    if (m_min > other.m_max)
    {
        return;
    }
    if (m_max < other.m_min)
    {
        return;
    }

    int dmin1, dmin2, bmin, dmax1, dmax2, bmax;
    if (m_min > other.m_min)
    {
        dmin1 = dataIndex(m_min);
        dmin2 = other.dataIndex(m_min);
    }
    else
    {
        dmin1 = dataIndex(other.m_min);
        dmin2 = other.dataIndex(other.m_min);
    }
    if (m_max < other.m_max)
    {
        dmax1 = dataIndex(m_max);
        dmax2 = other.dataIndex(m_max);
    }
    else
    {
        dmax1 = dataIndex(other.m_max);
        dmax2 = other.dataIndex(other.m_max);
    }

    for (int i = dmin1, j = dmin2; i <= dmax1; ++i, ++j)
    {
        m_data[i] = m_data[i] || other.m_data[j];
    }
}

bool BitRange::get(int i) const
{
    if (i < m_min || i > m_max)
    {
        return false;
    }
    return m_data[dataIndex(i)];
}

void BitRange::set(int i, bool high)
{
    assert(i >= m_min && i <= m_max);
    m_data[dataIndex(i)] = high;
}

int BitRange::min() const
{
    return m_min;
}

int BitRange::max() const
{
    return m_max;
}

int BitRange::range() const
{
    return m_max - m_min + 1;
}

inline int BitRange::dataIndex(int i) const
{
    return i - m_min;
}
