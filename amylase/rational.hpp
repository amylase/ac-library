#ifndef AMYLASE_RATIONAL_HPP
#define AMYLASE_RATIONAL_HPP

#include <string>
#include <amylase/bigint>

namespace amylase {

bigint _gcd(const bigint& x, const bigint& y) {
    assert(x >= 0);
    assert(y >= 0);
    if (x == 0) {
        return y;
    }
    return _gcd(y % x, x);
}

bigint _lcm(const bigint& x, const bigint& y) { return x * y / _gcd(x, y); }

struct rational {
    bigint dividend;
    bigint divisor;  // always positive

    rational(): dividend(0), divisor(1) {}
    rational(const bigint& _dividend, const bigint& _divisor): dividend(_dividend), divisor(_divisor) {
        normalize();
    }
    template <class T, atcoder::internal::is_signed_int_t<T>* = nullptr>
    rational(T x): dividend(x), divisor(1) {
        normalize();
    }
    template <class T, atcoder::internal::is_unsigned_int_t<T>* = nullptr>
    rational(T x): dividend(x), divisor(1) {
        normalize();
    }

    rational operator+() const { return *this; }
    rational operator-() const { return rational(-dividend, divisor); }

    rational& operator+=(const rational& other) {
        const bigint lcm = _lcm(divisor, other.divisor);
        const bigint this_mul = lcm / divisor;
        const bigint other_mul = lcm / other.divisor;
        divisor = lcm;
        dividend = dividend * this_mul + other.dividend * other_mul;
        normalize();
        return *this;
    }

    rational& operator-=(const rational& other) {
        return (*this) += (-other);
    }

    rational& operator*=(const rational& other) {
        dividend *= other.dividend;
        divisor *= other.divisor;
        normalize();
        return *this;
    }

    rational& operator/=(const rational& other) {
        dividend *= other.divisor;
        divisor *= other.dividend;
        normalize();
        return *this;
    }

    rational operator+(const rational other) const {
        return rational(*this) += other;
    }

    rational operator-(const rational& other) const {
        return rational(*this) -= other;
    }

    rational operator*(const rational& other) {
        return rational(*this) *= other;
    }

    rational operator/(const rational& other) const {
        return rational(*this) /= other;
    }

    rational& operator++() { return (*this) += 1; }
    rational& operator--() { return (*this) += -1; }
    rational operator++(int) {
        rational res = *this;
        ++*this;
        return res;
    }
    rational operator--(int) {
        rational res = *this;
        --*this;
        return res;
    }

    rational pow(long long n) const {
        assert(0 <= n);
        rational x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }

    rational inv() const {
        return rational(divisor, dividend);
    }

    long double val() const {
        return dividend.val() / divisor.val();
    }

    std::string str_val() const {
        if (divisor == 1) {
            return dividend.str_val();
        } else {
            return dividend.str_val() + "/" + divisor.str_val();
        }
    }

    void normalize() {
        assert(divisor != bigint(0));
        bigint gcd = _gcd(divisor.abs(), dividend.abs());
        divisor /= gcd;
        dividend /= gcd;
        if (divisor < 0) {
            divisor = -divisor;
            dividend = -dividend;
        }
    }
};

bool operator<(const rational& p, const rational& q) {
    if (p.dividend.sign != q.dividend.sign) {
        return p.dividend.sign < q.dividend.sign;
    } else {
        return p.dividend * q.divisor < q.dividend * p.divisor;
    }
}
bool operator>(const rational& p, const rational& q) { return q < p; }
bool operator<=(const rational& p, const rational& q) { return !(p > q); }
bool operator>=(const rational& p, const rational& q) { return !(p < q); }

bool operator==(const rational& p, const rational& q) {
    return p.dividend == q.dividend && p.divisor == q.divisor;
}
bool operator!=(const rational& p, const rational& q) { return !(p == q); }

}  // namespace amylase

#endif  // AMYLASE_RATIONAL_HPP
