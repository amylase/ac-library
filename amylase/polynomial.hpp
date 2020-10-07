#ifndef AMYLASE_POLYNOMIAL_HPP
#define AMYLASE_POLYNOMIAL_HPP 1

#include <vector>
#include <atcoder/modint>
#include <atcoder/convolution>

namespace amylase {

template <class T>
struct polynomial {
    std::vector<T> coef;

    polynomial(): coef() {}
    polynomial(const T x): coef(1, x) {}
    explicit polynomial(const std::vector<T>& _coef): coef(_coef) {}

    // add
    polynomial<T>& operator+=(const polynomial<T>& other) {
        while (coef.size() < other.coef.size()) {
            coef.push_back(0);
        }
        for (unsigned int d = 0; d < coef.size(); ++d) {
            coef[d] += other.coef[d];
        }
        normalize();
        return *this;
    }

    polynomial<T> operator+(const polynomial<T>& q) {
        return polynomial<T>(*this) += q;
    }

    // subtract
    polynomial<T>& operator-=(const polynomial<T>& other) {
        while (coef.size() < other.coef.size()) {
            coef.push_back(0);
        }
        for (unsigned int d = 0; d < coef.size(); ++d) {
            coef[d] -= other.coef[d];
        }
        normalize();
        return *this;
    }

    polynomial<T> operator-(const polynomial<T>& q) {
        return polynomial<T>(*this) -= q;
    }

    // substitute
    T substitute(const T x) {
        if (coef.size() == 0) {
            return 0;
        }
        T v = coef[0];
        T p = x;
        for (unsigned int i = 1; i < coef.size(); ++i) {
            v += coef[i] * p;
            p *= x;
        }
        return v;
    }

    // multiply
    template <class mint = T, atcoder::internal::is_static_modint_t<mint>* = nullptr>
    polynomial<mint>& operator*=(const polynomial<mint>& q) {
        std::vector<mint> result = atcoder::convolution(coef, q.coef);
        coef = result;
        normalize();
        return *this;
    }

    polynomial<long long>& operator*=(const polynomial<long long>& q) {
        std::vector<long long> result = atcoder::convolution_ll(coef, q.coef);
        coef = result;
        normalize();
        return *this;
    }

    polynomial<int>& operator*=(const polynomial<int>& q) {
        std::vector<long long> p_coef(coef.begin(), coef.end());
        std::vector<long long> q_coef(q.coef.begin(), q.coef.end());
        std::vector<long long> result = atcoder::convolution_ll(p_coef, q_coef);
        coef = std::vector<int>(result.begin(), result.end());
        normalize();
        return *this;
    }

    polynomial<T> operator*(const polynomial<T>& q) {
        return polynomial<T>(*this) *= q;
    }

    unsigned int degree() {
        return coef.size() + 1;
    }

    polynomial<T> derivative() {
        if (coef.size() == 0) {
            return polynomial<T>();
        }
        std::vector<T> new_coef(coef.size() - 1);
        for (unsigned int i = 0; i < new_coef.size(); ++i) {
            new_coef[i] = coef[i + 1] * (i + 1);
        }
        return polynomial<T>(new_coef);
    }

    polynomial<T>& normalize() {
        while (!coef.empty() && coef.back() == 0) {
            coef.pop_back();
        }
        return *this;
    }
};

template <class T>
bool operator==(const polynomial<T>& p, const polynomial<T>& q) {
    return p.coef == q.coef;
}

}  // namespace amylase

#endif  // AMYLASE_POLYNOMIAL_HPP
