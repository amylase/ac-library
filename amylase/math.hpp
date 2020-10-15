#ifndef AMYLASE_MATH_HPP
#define AMYLASE_MATH_HPP 1

#include <vector>

namespace amylase {

using bigint = __uint128_t;
long long _powmod(const long long x, const long long n, const long long mod) {
    if (n == 0) {
        return 1;
    }
    bigint sq = _powmod(x, n / 2, mod);
    if (n & 1) {
        return (long long) (sq * sq % mod * x % mod);
    } else {
        return (long long) (sq * sq % mod);
    }
}

bool _miller_rabin(const long long x, const std::vector<long long>& witnesses) {
    if (x <= 1) {
        return false;
    }
    if ((x & 1) == 0) {
        return x == 2;
    }
    long long d = x - 1;
    long long s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    for (const auto& a : witnesses) {
        if (a % x <= 1) {
            continue;
        }
        bool is_composite = true;
        is_composite &= _powmod(a, d, x) != 1;
        long long dd = d;
        for (int i = 0; i < s; ++i) {
            is_composite &= _powmod(a, dd, x) != x - 1;
            dd <<= 1;
        }
        if (is_composite) {
            return false;
        }
    }
    return true;
}

bool is_prime(const long long x) {
    return _miller_rabin(x, {2LL, 325LL, 9375LL, 28178LL, 450775LL, 9780504LL, 1795265022LL});
}

std::vector<long long> factor(long long x) {
    std::vector<long long> factors;
    long long p = 2;
    while (p * p <= x) {
        while (x % p == 0) {
            factors.emplace_back(p);
            x /= p;
        }
        p++;
    }
    if (x > 1) {
        factors.emplace_back(x);
    }
    return factors;
}

/**
 * solves ax + by = gcd(a, b)
 * @param a
 * @param b
 * @param x ref to variable to store root x
 * @param y ref to variable to store root x
 * @return gcd(a, b)
 */
long long extgcd(long long a, long long b, long long& x, long long& y) {
    long long d = a;
    if (b != 0) {
        d = extgcd(b, a % b, y, x);
        y -= (a / b) * x;
    } else {
        x = 1; y = 0;
    }
    return d;
}

}  // namespace amylase

#endif  // AMYLASE_MATH_HPP
