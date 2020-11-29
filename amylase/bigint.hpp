#ifndef AMYLASE_BIGINT_HPP
#define AMYLASE_BIGINT_HPP 1

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <atcoder/internal_type_traits.hpp>
#include <atcoder/convolution>

namespace amylase {

using _ull = unsigned long long;
const _ull _width = 4;
const _ull _size = (_ull) pow(10, _width);

_ull _get_lower(const _ull x) {
    return x % _size;
}

_ull _get_upper(const _ull x) {
    return x / _size;
}

struct bigint;
bool operator==(const bigint& p, const bigint& q);
bool operator!=(const bigint& p, const bigint& q);
bool operator<=(const bigint& p, const bigint& q);
bool operator>=(const bigint& p, const bigint& q);
bool operator>(const bigint& p, const bigint& q);
bool operator<(const bigint& p, const bigint& q);

struct bigint {
    int sign;  // either 1 or -1
    std::vector<_ull> data;

    bigint(): sign(1), data(0) {}
    bigint(const int _sign, const std::vector<_ull> _data): sign(_sign), data(_data) {
        normalize();
    }
    template <class T, atcoder::internal::is_signed_int_t<T>* = nullptr>
    bigint(T x) {
        if (x < 0) {
            sign = -1;
            x = std::abs(x);
        } else {
            sign = 1;
        }
        data = std::vector<_ull>(1, x);
        normalize();
    }
    template <class T, atcoder::internal::is_unsigned_int_t<T>* = nullptr>
    bigint(T x): sign(1), data(1, x) {
        normalize();
    }

    bigint operator+() const { return *this; }
    bigint operator-() const { return bigint(-sign, data); }

    bigint& operator+=(const bigint& other) {
        if (other == 0) {
            return (*this);
        }
        if (sign != other.sign) {
            return *this -= (-other);
        }

        const unsigned long long new_size = std::max(data.size(), other.data.size());
        data.resize(new_size, 0);
        for (unsigned int i = 0; i < other.data.size(); ++i) {
            data[i] += other.data[i];
        }
        normalize();
        return (*this);
    }

    bigint& operator-=(const bigint& other) {
        if (other == 0) {
            return (*this);
        }
        if (sign != other.sign) {
            return *this += (-other);
        }
        bigint p = this->abs();
        bigint q = other.abs();
        if (p < q) {
            sign *= -1;
            std::swap(p, q);
        }
        // p - q;
        std::vector<long long> d;
        long long carry = 0;
        for (unsigned int i = 0; i < std::max(p.data.size(), q.data.size()); ++i) {
            long long pv = i < p.data.size() ? p.data[i] : 0;
            long long qv = i < q.data.size() ? q.data[i] : 0;
            long long dv = pv + carry - qv;
            assert(dv + (long long)_size >= 0);
            if (dv >= 0) {
                d.emplace_back(dv);
                carry = 0;
            } else {
                d.emplace_back(dv + _size);
                carry = -1;
            }
        }
        assert(carry == 0);
        data = std::vector<_ull>(d.begin(), d.end());
        normalize();
        return (*this);
    }

    bigint& operator*=(const bigint& other) {
        std::vector<long long> p(data.begin(), data.end());
        std::vector<long long> q(other.data.begin(), other.data.end());
        std::vector<long long> conv = atcoder::convolution_ll(p, q);
        sign *= other.sign;
        data = std::vector<_ull>(conv.begin(), conv.end());
        normalize();
        return (*this);
    }

    bigint& _div2() {
        _ull carry = 0;
        for (int i = ((int) data.size()) - 1; i >= 0; --i) {
            carry *= _size;
            carry += data[i];
            data[i] = carry / 2;
            carry %= 2;
        }
        normalize();
        return (*this);
    }

    bigint& operator/=(const bigint& other) {
        assert(other != 0);
        bigint a(1, data);
        bigint b(1, other.data);

        bigint p = 0, q = a + 1;
        while (q - p > 1) {
            bigint m = (p + q)._div2();
            if (m * b <= a) {
                p = m;
            } else {
                q = m;
            }
        }
        sign *= other.sign;
        data = p.data;
        normalize();
        return (*this);
    }

    bigint& operator%=(const bigint& other) {
        return (*this) -= ((*this) / other) * other;
    }

    friend bigint operator+(const bigint& lhs, const bigint& rhs) {
        return bigint(lhs) += rhs;
    }
    friend bigint operator-(const bigint& lhs, const bigint& rhs) {
        return bigint(lhs) -= rhs;
    }
    friend bigint operator*(const bigint& lhs, const bigint& rhs) {
        return bigint(lhs) *= rhs;
    }
    friend bigint operator/(const bigint& lhs, const bigint& rhs) {
        return bigint(lhs) /= rhs;
    }
    friend bigint operator%(const bigint& lhs, const bigint& rhs) {
        return bigint(lhs) %= rhs;
    }

    bigint& operator++() { return (*this) += 1; }
    bigint& operator--() { return (*this) += -1; }
    bigint operator++(int) {
        bigint res = *this;
        ++*this;
        return res;
    }
    bigint operator--(int) {
        bigint res = *this;
        --*this;
        return res;
    }

    bigint abs() const { return (sign == 1) ? (*this) : -(*this); }

    long double val() const {
        long double v = 0;
        for (unsigned int i = 0; i < data.size(); ++i) {
            v += data[i];
            v *= _size;
        }
        return v;
    }

    std::string str_val() const {
        if (data.size() == 0) {
            return "0";
        }
        std::stringstream builder;
        if (sign == -1) {
            builder << "-";
        }
        auto itr = data.rbegin();
        builder << *itr;
        itr++;
        for (; itr != data.rend(); itr++) {
            builder << std::setw(_width) << std::setfill('0') << std::right << *itr;
        }
        return builder.str();
    }

    void normalize() {
        _ull carry = 0;
        unsigned int i = 0;
        while (i < data.size() || carry != 0) {
            if (i >= data.size()) {
                data.emplace_back(0);
            }
            carry += data[i];
            data[i] = _get_lower(carry);
            carry = _get_upper(carry);
            i++;
        }
        while (!data.empty() && data.back() == 0) {
            data.pop_back();
        }
        if (data.empty()) {
            sign = 1;
        }
    }
};

bool operator<(const bigint& p, const bigint& q) {
    if (p.sign != q.sign) {
        return p.sign < q.sign;
    } else {
        bool is_negative = p.sign < 0;
        if (p.data.size() != q.data.size()) {
            return (p.data.size() < q.data.size()) != is_negative;
        }
        std::vector<_ull> pd(p.data.rbegin(), p.data.rend());
        std::vector<_ull> qd(q.data.rbegin(), q.data.rend());
        if (is_negative) {
            return pd > qd;
        } else {
            return pd < qd;
        }
    }
}
bool operator>(const bigint& p, const bigint& q) { return q < p; }
bool operator<=(const bigint& p, const bigint& q) { return !(p > q); }
bool operator>=(const bigint& p, const bigint& q) { return !(p < q); }

bool operator==(const bigint& p, const bigint& q) {
    return p.sign == q.sign && p.data == q.data;
}
bool operator!=(const bigint& p, const bigint& q) { return !(p == q); }

}  // namespace amylase

#endif  // AMYLASE_BIGINT_HPP