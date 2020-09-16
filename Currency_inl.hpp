// Currency_inl.hpp
//////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////////
// UnsignedCurrency

inline significand_t UnsignedCurrency::upow10(exp10_t e10)
{
    assert(e10 >= 0);

    if (e10 == 0)
        return 1;

    significand_t ret = upow10(e10 >> 1);
    if (__builtin_mul_overflow(ret, ret, &ret))
        return max_significand;

    if (e10 & 1)
    {
        if (__builtin_mul_overflow(ret, 10, &ret))
            return max_significand;
    }

    return ret;
}

inline std::string
UnsignedCurrency::add_helper(std::string a, std::string b)
{
    assert(a.size() == b.size());

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    bool carry = false;
    for (size_t i = 0; i < a.size(); ++i)
    {
        a[i] = char(a[i] + b[i] + carry - '0');
        carry = (a[i] > '9');
        if (carry)
        {
            a[i] -= 10;
        }
    }

    if (carry)
    {
        a += char('0' + carry);
    }

    std::reverse(a.begin(), a.end());
    return a;
}

inline std::string
UnsignedCurrency::sub_helper(std::string a, std::string b)
{
    assert(a.size() == b.size());

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    bool borrow = false;
    for (size_t i = 0; i < a.size(); ++i)
    {
        a[i] = char(a[i] - b[i] - borrow + '0');
        borrow = (a[i] < '0');
        if (borrow)
        {
            a[i] += 10;
        }
    }

    if (borrow)
    {
        throw std::runtime_error("UnsignedCurrency::sub_helper");
    }

    std::reverse(a.begin(), a.end());
    return a;
}

inline void UnsignedCurrency::normalize()
{
    if (m_significand == 0)
    {
        m_exp10 = 0;
        assert(is_normalized());
        return;
    }

    while (m_significand % 10 == 0)
    {
        m_significand /= 10;
        ++m_exp10;
    }

    assert(is_normalized());
}

inline void UnsignedCurrency::parse(const char *str)
{
    m_significand = 0;
    m_exp10 = 0;

    while (*str == ' ')
        ++str;

    if (std::strcmp(str, "inf") == 0)
    {
        set_inf();
        return;
    }

    while (*str == '0')
        ++str;

    for (bool found_dot = false; *str; ++str)
    {
        if (*str == '.')
        {
            found_dot = true;
            continue;
        }

        if (found_dot)
        {
            if (__builtin_sub_overflow(m_exp10, 1, &m_exp10))
            {
                ++m_exp10;
                normalize();
                return;
            }
        }

        if ('0' <= *str && *str <= '9')
        {
            significand_t save = m_significand;
            if (__builtin_mul_overflow(m_significand, 10, &m_significand))
            {
                if (m_exp10 == 0)
                {
                    set_inf();
                }
                else
                {
                    m_significand = save;
                    ++m_exp10;
                }
                normalize();
                return;
            }
            if (__builtin_add_overflow(m_significand, *str - '0', &m_significand))
            {
                if (m_exp10 == 0)
                {
                    set_inf();
                }
                else
                {
                    m_significand = save;
                    ++m_exp10;
                }
                normalize();
                return;
            }
        }
        else
        {
            throw std::runtime_error("UnsignedCurrency::UnsignedCurrency: invalid character");
        }
    }

    normalize();
}

inline UnsignedCurrency::UnsignedCurrency(double value)
{
    if (value < 0)
        throw std::runtime_error("UnsignedCurrency::UnsignedCurrency(double)");

    char buf[64];
    std::sprintf(buf, "%.20f", value);
    parse(buf);
}

inline bool UnsignedCurrency::equals(const UnsignedCurrency& another) const
{
    assert(is_normalized());
    assert(another.is_normalized());
    return std::memcmp(this, &another, sizeof(another)) == 0;
}

inline int UnsignedCurrency::compare(const UnsignedCurrency& another) const
{
    assert(is_normalized());
    assert(another.is_normalized());

    if (another.is_zero())
        return !is_zero();

    if (is_zero())
        return -1;

    if (m_exp10 == another.m_exp10)
    {
        if (m_significand < another.m_significand)
            return -1;
        if (m_significand > another.m_significand)
            return 1;
        return 0;
    }

    std::string a = std::to_string(m_significand);
    std::string b = std::to_string(another.m_significand);

    if (m_exp10 < another.m_exp10)
    {
        std::string zeros(another.m_exp10 - m_exp10, '0');
        b += zeros;
    }
    else
    {
        std::string zeros(m_exp10 - another.m_exp10, '0');
        a += zeros;
    }

    if (a.size() != b.size())
    {
        if (a.size() < b.size())
        {
            std::string zeros(b.size() - a.size(), '0');
            a = zeros + a;
        }
        else
        {
            std::string zeros(a.size() - b.size(), '0');
            b = zeros + b;
        }
    }

    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

inline UnsignedCurrency&
UnsignedCurrency::operator+=(const UnsignedCurrency& another)
{
    assert(is_normalized());
    assert(another.is_normalized());

    if (another.is_zero())
        return *this;

    if (is_zero())
        return (*this = another);

    if (m_exp10 == another.m_exp10)
    {
        m_significand += another.m_significand;
        normalize();
        return *this;
    }

    std::string a = std::to_string(m_significand);
    std::string b = std::to_string(another.m_significand);

    exp10_t min_exp10;
    if (m_exp10 < another.m_exp10)
    {
        min_exp10 = m_exp10;
        std::string zeros(another.m_exp10 - m_exp10, '0');
        b += zeros;
    }
    else
    {
        min_exp10 = another.m_exp10;
        std::string zeros(m_exp10 - another.m_exp10, '0');
        a += zeros;
    }

    if (a.size() != b.size())
    {
        if (a.size() < b.size())
        {
            std::string zeros(b.size() - a.size(), '0');
            a = zeros + a;
        }
        else
        {
            std::string zeros(a.size() - b.size(), '0');
            b = zeros + b;
        }
    }

    std::string c = add_helper(a, b);
    parse(c.c_str());
    m_exp10 += min_exp10;

    normalize();
    return *this;
}

inline UnsignedCurrency&
UnsignedCurrency::operator-=(const UnsignedCurrency& another)
{
    assert(is_normalized());
    assert(another.is_normalized());

    if (m_exp10 == another.m_exp10)
    {
        if (m_significand == another.m_significand)
        {
            clear();
            return *this;
        }
        if (m_significand < another.m_significand)
            throw std::runtime_error("UnsignedCurrency::operator-=");
        m_significand -= another.m_significand;
        normalize();
        return *this;
    }

    std::string a = std::to_string(m_significand);
    std::string b = std::to_string(another.m_significand);

    exp10_t min_exp10;
    if (m_exp10 < another.m_exp10)
    {
        min_exp10 = m_exp10;
        std::string zeros(another.m_exp10 - m_exp10, '0');
        b += zeros;
    }
    else
    {
        min_exp10 = another.m_exp10;
        std::string zeros(m_exp10 - another.m_exp10, '0');
        a += zeros;
    }

    if (a.size() != b.size())
    {
        if (a.size() < b.size())
        {
            std::string zeros(b.size() - a.size(), '0');
            a = zeros + a;
        }
        else
        {
            std::string zeros(a.size() - b.size(), '0');
            b = zeros + b;
        }
    }

    if (a < b)
        throw std::runtime_error("UnsignedCurrency::operator-=");

    std::string c = sub_helper(a, b);
    parse(c.c_str());
    m_exp10 += min_exp10;

    normalize();
    return *this;
}

inline UnsignedCurrency& UnsignedCurrency::operator*=(const UnsignedCurrency& another)
{
    assert(is_normalized());
    assert(another.is_normalized());

    if (is_zero() || another.is_zero())
    {
        clear();
        return *this;
    }

    significand_t significand;
    while (__builtin_mul_overflow(m_significand, another.m_significand, &significand))
    {
        m_significand /= 10;
        if (__builtin_add_overflow(m_exp10, 1, &m_exp10))
        {
            set_inf();
            return *this;
        }
    }
    m_significand = significand;

    if (__builtin_add_overflow(m_exp10, another.m_exp10, &m_exp10))
    {
        set_inf();
        return *this;
    }

    normalize();
    return *this;
}

inline std::string UnsignedCurrency::to_string() const
{
    assert(is_normalized());

    if (is_zero())
        return "0";

    if (is_inf())
        return "inf";

    std::string ret = std::to_string(m_significand);
    if (m_exp10 == 0)
        return ret; // just a integer

    if (m_exp10 > 0)
    {
        std::string zeros(m_exp10, '0');
        ret += zeros;
        return ret; // 123456000...0
    }

    if (-m_exp10 < ret.size())
    {
        // 1234.56...
        ret.insert(ret.size() + m_exp10, ".");
        return ret;
    }

    // 0.0000123456...
    std::string str("0.");
    std::string zeros(-m_exp10 - ret.size(), '0');
    str += zeros;
    str += ret;
    return str;
}

inline UnsignedCurrency&
UnsignedCurrency::operator/=(const UnsignedCurrency& another)
{
    UnsignedCurrency rhs(another);
    rhs.invert();
    return (*this *= rhs);
}

inline void UnsignedCurrency::invert()
{
    Currency cur(*this);
    cur.invert();
    *this = cur.base();
}

inline UnsignedCurrency UnsignedCurrency::get_inverted() const
{
    UnsignedCurrency ret(*this);
    ret.invert();
    return ret;
}

inline UnsignedCurrency::operator double() const
{
    double ret = m_significand;
    if (m_exp10 >= 0)
        ret *= upow10(m_exp10);
    else
        ret /= upow10(-m_exp10);
    return ret;
}

inline void UnsignedCurrency::set_inf()
{
    m_significand = max_significand;
    m_exp10 = max_exp10;
}

inline bool UnsignedCurrency::is_inf() const
{
    return m_significand == max_significand && m_exp10 == max_exp10;
}

//////////////////////////////////////////////////////////////////////////////
// Currency

inline void Currency::parse(const char *str)
{
    while (*str == ' ')
        ++str;

    m_negative = (*str == '-');
    if (m_negative || *str == '+')
        ++str;

    m_base.parse(str);
    normalize();
}

inline Currency::Currency(double value)
{
    char buf[64];
    std::sprintf(buf, "%.20f", value);
    parse(buf);
}

inline Currency Currency::operator-() const
{
    assert(is_normalized());
    if (is_zero())
        return *this;

    Currency ret = *this;
    ret.m_negative = !ret.m_negative;
    assert(is_normalized());
    return ret;
}

inline bool Currency::equals(const Currency& another) const
{
    assert(is_normalized());
    assert(another.is_normalized());
    if (m_negative != another.m_negative)
        return false;
    return m_base.equals(another.m_base);
}

inline int Currency::compare(const Currency& another) const
{
    assert(is_normalized());
    assert(another.is_normalized());
    if (m_negative)
    {
        if (another.m_negative)
            return -m_base.compare(another.m_base);
        return -1;
    }
    else
    {
        if (another.m_negative)
            return 1;
        return m_base.compare(another.m_base);
    }
}

inline Currency& Currency::operator+=(const Currency& another)
{
    if (m_negative == another.m_negative)
    {
        m_base += another.m_base;
    }
    else
    {
        int base_comp = m_base.compare(another.m_base);
        if (base_comp == 0)
        {
            clear();
            return *this;
        }
        if (base_comp > 0)
        {
            m_base -= another.m_base;
        }
        else
        {
            m_base = another.m_base - m_base;
            m_negative = another.m_negative;
        }
    }
    normalize();
    return *this;
}

inline Currency& Currency::operator-=(const Currency& another)
{
    return (*this += (-another));
}

inline Currency& Currency::operator*=(const Currency& another)
{
    assert(is_normalized());
    assert(another.is_normalized());

    m_negative = (m_negative != another.m_negative);
    m_base *= another.m_base;
    normalize();
    return *this;
}

inline std::string Currency::to_string() const
{
    if (is_zero())
        return "0";

    if (is_negative())
    {
        std::string ret("-");
        ret += m_base.to_string();
        return ret;
    }

    return m_base.to_string();
}

inline Currency&
Currency::operator/=(const Currency& another)
{
    Currency rhs(another);
    rhs.invert();
    return (*this *= rhs);
}

inline void Currency::invert()
{
    base_t a = base();
    if (a <= base_t::epsilon)
    {
        set_inf();
        return;
    }

    const Currency alpha(a), one(1), two(2);
    Currency x(1 / (double)a), y, h;

    // TODO:

    base() = x.base();
}

inline Currency Currency::get_inverted() const
{
    Currency ret(*this);
    ret.invert();
    return ret;
}

inline Currency::operator double() const
{
    double ret = (double)m_base;
    if (m_negative)
        return -ret;
    return ret;
}

inline void Currency::set_inf(bool negative)
{
    m_base.set_inf();
    m_negative = negative;
}

inline bool Currency::is_inf() const
{
    return m_base.is_inf();
}
