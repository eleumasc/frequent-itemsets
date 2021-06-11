#pragma once

#include <vector>

class BitRange
{
public:
    BitRange();

    BitRange(int min, int max);

    void join(const BitRange &other);

    bool get(int i) const;

    void set(int i, bool high = true);

    int min() const;

    int max() const;

    int range() const;

private:
    std::vector<bool> m_data;

    int m_min;

    int m_max;

    int dataIndex(int i) const;
};
