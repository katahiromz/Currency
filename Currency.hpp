// Currency.hpp
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <algorithm>

namespace khmz
{

typedef int64_t significand_t;
typedef int32_t exp10_t;

static const significand_t max_significand = std::numeric_limits<significand_t>::max();
static const exp10_t max_exp10 = std::numeric_limits<exp10_t>::max();

//////////////////////////////////////////////////////////////////////////////
// UnsignedCurrency

class UnsignedCurrency
{
protected:
    significand_t m_significand;
    exp10_t m_exp10;
    static significand_t upow10(exp10_t e10);
    static std::string add_helper(std::string a, std::string b);
    static std::string sub_helper(std::string a, std::string b);

public:
    UnsignedCurrency()
        : m_significand(0)
        , m_exp10(0)
    {
    }

    bool is_normalized() const
    {
        if (is_inf())
            return true;

        if (m_significand == 0)
            return m_exp10 == 0;

        return m_significand % 10 != 0;
    }

    void normalize();

    explicit UnsignedCurrency(significand_t significand, exp10_t exp10 = 0)
        : m_significand(significand)
        , m_exp10(exp10)
    {
        normalize();
    }

    explicit UnsignedCurrency(int significand, exp10_t exp10 = 0)
        : UnsignedCurrency(significand_t(significand), exp10)
    {
    }
    explicit UnsignedCurrency(unsigned int significand, exp10_t exp10 = 0)
        : UnsignedCurrency(significand_t(significand), exp10)
    {
    }

    explicit UnsignedCurrency(long significand, exp10_t exp10 = 0)
        : UnsignedCurrency(significand_t(significand), exp10)
    {
    }
    explicit UnsignedCurrency(unsigned long significand, exp10_t exp10 = 0)
        : UnsignedCurrency(significand_t(significand), exp10)
    {
    }

    UnsignedCurrency(const UnsignedCurrency&) = default;
    UnsignedCurrency& operator=(const UnsignedCurrency&) = default;

    void parse(const char *str);

    UnsignedCurrency(const char *str)
    {
        parse(str);
    }
    UnsignedCurrency(const std::string& str)
    {
        parse(str.c_str());
    }
    explicit UnsignedCurrency(double value);

    UnsignedCurrency operator+() const
    {
        return *this;
    }

    bool is_zero() const
    {
        return m_significand == 0;
    }
    bool operator!() const
    {
        return is_zero();
    }
    void clear()
    {
        m_significand = 0;
        m_exp10 = 0;
        assert(is_normalized());
    }

    int compare(const UnsignedCurrency& another) const;
    bool equals(const UnsignedCurrency& another) const;

    friend bool operator==(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        return lhs.compare(rhs) == 0;
    }
    friend bool operator==(const char *lhs, const UnsignedCurrency& rhs)
    {
        return UnsignedCurrency(lhs).compare(rhs) == 0;
    }
    friend bool operator==(const UnsignedCurrency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) == 0;
    }

    friend bool operator!=(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        return lhs.compare(rhs) != 0;
    }
    friend bool operator!=(const char *lhs, const UnsignedCurrency& rhs)
    {
        return UnsignedCurrency(lhs).compare(rhs) != 0;
    }
    friend bool operator!=(const UnsignedCurrency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) != 0;
    }

    friend bool operator<(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        return lhs.compare(rhs) < 0;
    }
    friend bool operator<(const char *lhs, const UnsignedCurrency& rhs)
    {
        return UnsignedCurrency(lhs).compare(rhs) < 0;
    }
    friend bool operator<(const UnsignedCurrency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) < 0;
    }

    friend bool operator>(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        return lhs.compare(rhs) > 0;
    }
    friend bool operator>(const char *lhs, const UnsignedCurrency& rhs)
    {
        return UnsignedCurrency(lhs).compare(rhs) > 0;
    }
    friend bool operator>(const UnsignedCurrency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) > 0;
    }

    friend bool operator<=(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        return lhs.compare(rhs) <= 0;
    }
    friend bool operator<=(const char *lhs, const UnsignedCurrency& rhs)
    {
        return UnsignedCurrency(lhs).compare(rhs) <= 0;
    }
    friend bool operator<=(const UnsignedCurrency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) <= 0;
    }

    friend bool operator>=(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        return lhs.compare(rhs) >= 0;
    }
    friend bool operator>=(const char *lhs, const UnsignedCurrency& rhs)
    {
        return UnsignedCurrency(lhs).compare(rhs) >= 0;
    }
    friend bool operator>=(const UnsignedCurrency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) >= 0;
    }

    UnsignedCurrency& operator+=(const UnsignedCurrency& another);
    UnsignedCurrency& operator-=(const UnsignedCurrency& another);
    UnsignedCurrency& operator*=(const UnsignedCurrency& another);
    UnsignedCurrency& operator/=(const UnsignedCurrency& another);

    friend UnsignedCurrency
    operator+(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret += rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator+(const char *lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret += rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator+(const UnsignedCurrency& lhs, const char *rhs)
    {
        UnsignedCurrency ret(lhs);
        ret += UnsignedCurrency(rhs);
        return ret;
    }
    friend UnsignedCurrency
    operator-(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret -= rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator-(const char *lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret -= rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator-(const UnsignedCurrency& lhs, const char *rhs)
    {
        UnsignedCurrency ret(lhs);
        ret -= UnsignedCurrency(rhs);
        return ret;
    }
    friend UnsignedCurrency
    operator*(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret *= rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator*(const char *lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret *= rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator*(const UnsignedCurrency& lhs, const char *rhs)
    {
        UnsignedCurrency ret(lhs);
        ret *= UnsignedCurrency(rhs);
        return ret;
    }
    friend UnsignedCurrency
    operator/(const UnsignedCurrency& lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret /= rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator/(const char *lhs, const UnsignedCurrency& rhs)
    {
        UnsignedCurrency ret(lhs);
        ret /= rhs;
        return ret;
    }
    friend UnsignedCurrency
    operator/(const UnsignedCurrency& lhs, const char *rhs)
    {
        UnsignedCurrency ret(lhs);
        ret /= UnsignedCurrency(rhs);
        return ret;
    }

    void invert();
    UnsignedCurrency get_inverted() const;

    std::string to_string() const;

    const char *c_str() const
    {
        return to_string().c_str();
    }

    void print() const
    {
        std::printf("%s\n", to_string().c_str());
    }

    operator double() const;

    bool is_inf() const;
    void set_inf();

    static void unittest();

    friend UnsignedCurrency abs(const UnsignedCurrency& cur)
    {
        return cur;
    }

    static const UnsignedCurrency epsilon;
};


//////////////////////////////////////////////////////////////////////////////
// Currency

class Currency
{
protected:
    typedef UnsignedCurrency base_t;
    base_t m_base;
    bool m_negative;

public:
    Currency()
        : m_base()
        , m_negative(false)
    {
    }

    Currency(const Currency&) = default;
    Currency& operator=(const Currency&) = default;

    bool is_normalized() const
    {
        if (m_base.is_inf())
            return true;

        if (m_base.is_zero() && m_negative)
            return false;
        return m_base.is_normalized();
    }

    void normalize()
    {
        if (is_zero())
            m_negative = false;
        m_base.normalize();
        assert(is_normalized());
    }

    base_t& base()
    {
        return m_base;
    }
    const base_t& base() const
    {
        return m_base;
    }

    explicit Currency(significand_t significand, exp10_t exp10 = 0)
        : m_base(std::abs(significand), exp10)
        , m_negative(significand < 0)
    {
        normalize();
    }
    explicit Currency(int significand, exp10_t exp10 = 0)
        : Currency(significand_t(significand), exp10)
    {
    }
    explicit Currency(unsigned int significand, exp10_t exp10 = 0)
        : Currency(significand_t(significand), exp10)
    {
    }
    explicit Currency(long significand, exp10_t exp10 = 0)
        : Currency(significand_t(significand), exp10)
    {
    }
    explicit Currency(unsigned long significand, exp10_t exp10 = 0)
        : Currency(significand_t(significand), exp10)
    {
    }
    explicit Currency(double value);

    void parse(const char *str);

    Currency(const char *str)
    {
        parse(str);
    }
    Currency(const std::string& str)
    {
        parse(str.c_str());
    }

    Currency(const base_t& base, bool negative = false)
        : m_base(base), m_negative(negative)
    {
        normalize();
    }

    Currency operator+() const
    {
        return *this;
    }
    Currency operator-() const;

    bool is_zero() const
    {
        return m_base.is_zero();
    }
    bool is_positive() const
    {
        return !is_zero() && !m_negative;
    }
    bool is_negative() const
    {
        return m_negative;
    }
    void set_negative(bool negative = true)
    {
        m_negative = negative;
    }
    bool operator!() const
    {
        return is_zero();
    }
    void clear()
    {
        m_base.clear();
        m_negative = false;
        assert(is_normalized());
    }

    int compare(const Currency& another) const;
    bool equals(const Currency& another) const;

    friend bool operator==(const Currency& lhs, const Currency& rhs)
    {
        return lhs.compare(rhs) == 0;
    }
    friend bool operator==(const char *lhs, const Currency& rhs)
    {
        return Currency(lhs).compare(rhs) == 0;
    }
    friend bool operator==(const Currency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) == 0;
    }

    friend bool operator!=(const Currency& lhs, const Currency& rhs)
    {
        return lhs.compare(rhs) != 0;
    }
    friend bool operator!=(const char *lhs, const Currency& rhs)
    {
        return Currency(lhs).compare(rhs) != 0;
    }
    friend bool operator!=(const Currency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) != 0;
    }

    friend bool operator<(const Currency& lhs, const Currency& rhs)
    {
        return lhs.compare(rhs) < 0;
    }
    friend bool operator<(const char *lhs, const Currency& rhs)
    {
        return Currency(lhs).compare(rhs) < 0;
    }
    friend bool operator<(const Currency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) < 0;
    }

    friend bool operator>(const Currency& lhs, const Currency& rhs)
    {
        return lhs.compare(rhs) > 0;
    }
    friend bool operator>(const char *lhs, const Currency& rhs)
    {
        return Currency(lhs).compare(rhs) > 0;
    }
    friend bool operator>(const Currency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) > 0;
    }

    friend bool operator<=(const Currency& lhs, const Currency& rhs)
    {
        return lhs.compare(rhs) <= 0;
    }
    friend bool operator<=(const char *lhs, const Currency& rhs)
    {
        return Currency(lhs).compare(rhs) <= 0;
    }
    friend bool operator<=(const Currency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) <= 0;
    }

    friend bool operator>=(const Currency& lhs, const Currency& rhs)
    {
        return lhs.compare(rhs) >= 0;
    }
    friend bool operator>=(const char *lhs, const Currency& rhs)
    {
        return Currency(lhs).compare(rhs) >= 0;
    }
    friend bool operator>=(const Currency& lhs, const char *rhs)
    {
        return lhs.compare(rhs) >= 0;
    }

    Currency& operator+=(const Currency& another);
    Currency& operator-=(const Currency& another);
    Currency& operator*=(const Currency& another);
    Currency& operator/=(const Currency& another);

    friend Currency
    operator+(const Currency& lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret += rhs;
        return ret;
    }
    friend Currency
    operator+(const char *lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret += rhs;
        return ret;
    }
    friend Currency
    operator+(const Currency& lhs, const char *rhs)
    {
        Currency ret(lhs);
        ret += Currency(rhs);
        return ret;
    }
    friend Currency
    operator-(const Currency& lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret -= rhs;
        return ret;
    }
    friend Currency
    operator-(const char *lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret -= rhs;
        return ret;
    }
    friend Currency
    operator-(const Currency& lhs, const char *rhs)
    {
        Currency ret(lhs);
        ret -= Currency(rhs);
        return ret;
    }
    friend Currency
    operator*(const Currency& lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret *= rhs;
        return ret;
    }
    friend Currency
    operator*(const char *lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret *= rhs;
        return ret;
    }
    friend Currency
    operator*(const Currency& lhs, const char *rhs)
    {
        Currency ret(lhs);
        ret *= Currency(rhs);
        return ret;
    }
    friend Currency
    operator/(const Currency& lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret /= rhs;
        return ret;
    }
    friend Currency
    operator/(const char *lhs, const Currency& rhs)
    {
        Currency ret(lhs);
        ret /= rhs;
        return ret;
    }
    friend Currency
    operator/(const Currency& lhs, const char *rhs)
    {
        Currency ret(lhs);
        ret /= Currency(rhs);
        return ret;
    }

    void invert();
    Currency get_inverted() const;

    std::string to_string() const;

    const char *c_str() const
    {
        return to_string().c_str();
    }

    void print() const
    {
        std::printf("%s\n", to_string().c_str());
    }

    void round(exp10_t e10 = 0)
    {
        // TODO:
    }
    void round_up(exp10_t e10 = 0)
    {
        // TODO:
    }
    void round_down(exp10_t e10 = 0)
    {
        // TODO:
    }
    void bankers_rounding(exp10_t e10 = 0)
    {
        // TODO:
    }

    Currency get_round(exp10_t e10 = 0)
    {
        Currency ret(*this);
        ret.round(e10);
        return ret;
    }
    Currency get_round_up(exp10_t e10 = 0)
    {
        Currency ret(*this);
        ret.round_up(e10);
        return ret;
    }
    Currency get_round_down(exp10_t e10 = 0)
    {
        Currency ret(*this);
        ret.round_down(e10);
        return ret;
    }
    Currency get_bankers_rounding(exp10_t e10)
    {
        Currency ret(*this);
        ret.get_bankers_rounding(e10);
        return ret;
    }

    operator double() const;

    bool is_inf() const;
    void set_inf(bool negative = false);

    static void unittest();

    friend Currency abs(const Currency& cur)
    {
        return Currency(cur.m_base);
    }

    static const Currency epsilon;
};

#include "Currency_inl.hpp"

} // namespace khmz

//////////////////////////////////////////////////////////////////////////////
