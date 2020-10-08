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
    explicit polynomial(const T x): coef(1, x) {}
    explicit polynomial(const std::vector<T>& _coef): coef(_coef) {
        normalize();
    }

    // add
    polynomial<T>& operator+=(const polynomial<T>& other) {
        while (coef.size() < other.coef.size()) {
            coef.push_back(0);
        }
        for (unsigned int d = 0; d < other.coef.size(); ++d) {
            coef[d] += other.coef[d];
        }
        normalize();
        return *this;
    }

    polynomial<T> operator+(const polynomial<T>& q) const {
        return polynomial<T>(*this) += q;
    }

    // subtract
    polynomial<T>& operator-=(const polynomial<T>& other) {
        while (coef.size() < other.coef.size()) {
            coef.push_back(0);
        }
        for (unsigned int d = 0; d < other.coef.size(); ++d) {
            coef[d] -= other.coef[d];
        }
        normalize();
        return *this;
    }

    polynomial<T> operator-(const polynomial<T>& q) const {
        return polynomial<T>(*this) -= q;
    }

    // substitute
    T substitute(const T x) const {
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
        static_assert(mint::mod() != 1000000007, "atcoder::convolution does not support mod 1000000007. use 998244353 instead.");
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

    polynomial<T> operator*(const polynomial<T>& q) const {
        return polynomial<T>(*this) *= q;
    }

    // divide
    polynomial<T> star() const {
        std::vector<T> new_coef(coef.rbegin(), coef.rend());
        return polynomial(new_coef);
    }

    polynomial<T>& operator/=(const polynomial<T>& g) {
        // http://www.math.kobe-u.ac.jp/Asir/ca.pdf pp.29

        const int q_degree = (int) degree() - g.degree();
        if (q_degree < 0) {
            coef.clear();
            return *this;
        }

        if (g.degree() < 50 || q_degree < 50) {
            std::vector<T> new_coef(q_degree + 1);
            for (int d = (int) q_degree; d >= 0; --d) {
                const T c = coef[d + g.degree()] / g.coef.back();
                new_coef[d] = c;
                for (unsigned int i = 0; i < g.degree(); ++i) {
                    coef[d + i] -= c * g.coef[i];
                }
            }
            coef = new_coef;
            return *this;
        }

        const polynomial<T> gstar = g.star();
        polynomial<T> t(std::vector<T>(1, 1 / gstar.coef[0]));
        while ((int) t.coef.size() < q_degree + 1) {
            const int next_t_size = (int) t.coef.size() * 2;
            t = t * polynomial<T>(2) - t * t * gstar;
            t.coef.resize(next_t_size);
        }

        auto qstar = t * this->star();
        qstar.coef.resize(q_degree + 1);
        coef = qstar.star().coef;
        return *this;
    }

    polynomial<T> operator/(const polynomial<T>& g) const {
        return polynomial<T>(*this) /= g;
    }

    polynomial<T>& operator%=(const polynomial<T>& g) {
        auto q = (*this) / g;
        coef = ((*this) - q * g).coef;
        return *this;
    }

    polynomial<T> operator%(const polynomial<T>& g) const {
        return polynomial<T>(*this) %= g;
    }

    unsigned int degree() const {
        if (coef.size() > 0) {
            return (int) coef.size() - 1;
        } else {
            return 0;
        }
    }

    polynomial<T> derivative() const {
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

using mod_polynomial = polynomial<atcoder::modint998244353>;

template <class T>
bool operator==(const polynomial<T>& p, const polynomial<T>& q) {
    return p.coef == q.coef;
}

}  // namespace amylase

#endif  // AMYLASE_POLYNOMIAL_HPP
