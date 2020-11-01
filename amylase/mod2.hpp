#ifndef AMYLASE_MOD2_HPP
#define AMYLASE_MOD2_HPP 1

#include <atcoder/modint>

namespace amylase {
// modulo-free specialization of static_mod where modulo = 2.
struct mod2 : atcoder::internal::static_modint_base {

  public:
    static constexpr int mod() { return 2; }
    static mod2 raw(int v) {
        mod2 x;
        x._v = v;
        return x;
    }

    mod2() : _v(0) {}
    template <class T, atcoder::internal::is_signed_int_t<T>* = nullptr>
    mod2(T v) {
        unsigned long long x;
        if (v >= 0) {
            x = v;
        } else {
            x = -v;
        }
        _v = (unsigned int)(x & 1u);
    }
    template <class T, atcoder::internal::is_unsigned_int_t<T>* = nullptr>
    mod2(T v) {
        _v = (unsigned int)(v & 1u);
    }
    mod2(bool v) { _v = ((unsigned int)(v) & 1u); }

    unsigned int val() const { return _v; }

    mod2& operator++() {
        _v = 1 - _v;
        return *this;
    }
    mod2& operator--() {
        return operator++();
    }
    mod2 operator++(int) {
        mod2 result = *this;
        ++*this;
        return result;
    }
    mod2 operator--(int) {
        mod2 result = *this;
        --*this;
        return result;
    }

    mod2& operator+=(const mod2& rhs) {
        _v += rhs._v;
        _v &= 1u;
        return *this;
    }
    mod2& operator-=(const mod2& rhs) {
        _v -= rhs._v;
        _v &= 1u;
        return *this;
    }
    mod2& operator*=(const mod2& rhs) {
        _v *= rhs._v;
        _v &= 1u;
        return *this;
    }
    mod2& operator/=(const mod2& rhs) { return *this = *this * rhs.inv(); }

    mod2 operator+() const { return *this; }
    mod2 operator-() const { return mod2() - *this; }

    mod2 pow(long long n) const {
        assert(0 <= n);
        if (_v) {
            return _v;
        } else {
            return n == 0;
        }
    }
    mod2 inv() const {
        assert(_v);
        return _v;
    }

    friend mod2 operator+(const mod2& lhs, const mod2& rhs) {
        return mod2(lhs) += rhs;
    }
    friend mod2 operator-(const mod2& lhs, const mod2& rhs) {
        return mod2(lhs) -= rhs;
    }
    friend mod2 operator*(const mod2& lhs, const mod2& rhs) {
        return mod2(lhs) *= rhs;
    }
    friend mod2 operator/(const mod2& lhs, const mod2& rhs) {
        return mod2(lhs) /= rhs;
    }
    friend bool operator==(const mod2& lhs, const mod2& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mod2& lhs, const mod2& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static constexpr unsigned int umod() { return 2; }
    static constexpr bool prime = true;
};

}  // namespace amylase

#endif  // AMYLASE_MOD2_HPP
