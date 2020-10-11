#ifndef AMYLASE_POLYNOMIAL_HPP
#define AMYLASE_POLYNOMIAL_HPP 1

#include <queue>
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

    T get(const unsigned int i) const {
        if (i < coef.size()) {
            return coef[i];
        } else {
            return 0;
        }
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

    // multiply (general case)
    // implementation
    polynomial<T>& multiply_slow(const polynomial<T>& q) {
        const unsigned int new_size = degree() + q.degree() + 1;
        if (std::min(degree(), q.degree()) < 50) {
            std::vector<T> p = coef;
            coef.resize(new_size);
            std::fill(coef.begin(), coef.end(), (T)0);
            for (unsigned int i = 0; i < p.size(); ++i) {
                for (unsigned int j = 0; j < q.coef.size(); ++j) {
                    coef[i + j] += p[i] * q.coef[j];
                }
            }
            normalize();
            return *this;
        }

        const int size = std::max(degree(), q.degree()) + 1;
        const int first_size = size / 2;
        const int second_size = size - first_size;
        std::vector<T> my_first(first_size), my_second(second_size);
        std::vector<T> op_first(first_size), op_second(second_size);
        for (int deg = 0; deg < size; ++deg) {
            if (deg < first_size) {
                my_first[deg] = get(deg);
                op_first[deg] = q.get(deg);
            } else {
                my_second[deg - first_size] = get(deg);
                op_second[deg - first_size] = q.get(deg);
            }
        }

        const polynomial<T> my_0(my_first), my_1(my_second), op_0(op_first),
            op_1(op_second);

        auto prod_0 = my_0 * op_0;
        auto prod_2 = my_1 * op_1;
        auto prod_1 = prod_0 + prod_2 - (my_1 - my_0) * (op_1 - op_0);

        auto prod =
            prod_2.shift(2 * first_size) + prod_1.shift(first_size) + prod_0;
        coef = prod.coef;
        return *this;
    }

    polynomial<T> shift(const unsigned int n) const {
        std::vector<T> new_coef(n, 0);
        for (unsigned int i = 0; i < coef.size(); ++i) {
            new_coef.push_back(coef[i]);
        }
        return polynomial<T>(new_coef);
    }

    // 1. non convolutional static_modint
    template <class mint = T, atcoder::internal::is_static_modint_t<mint>* = nullptr, std::enable_if_t<mint::mod() != 998244353>* = nullptr>
    polynomial<T>& operator*=(const polynomial<T>& q) {
        return multiply_slow(q);
    }

    // 2. other types
    template <class U = T, std::enable_if_t<!std::is_integral<U>::value && !atcoder::internal::is_static_modint_t<U>::value>* = nullptr>
    polynomial<U>& operator*=(const polynomial<U>& q) {
        return multiply_slow(q);
    }

    // multiply (special case)
    template <class mint = T, atcoder::internal::is_static_modint_t<mint>* = nullptr, std::enable_if_t<mint::mod() == 998244353>* = nullptr>
    polynomial<mint>& operator*=(const polynomial<mint>& q) {
        std::vector<T> result = atcoder::convolution(coef, q.coef);
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

    // divide (general case)
    polynomial<T>& divide_slow(const polynomial<T>& g) {
        const int q_degree = (int) degree() - g.degree();

        if (q_degree < 0) {
            coef.clear();
            return *this;
        }

        // r = coef
        std::vector<T> q(q_degree + 1);
        for (int d = (int) q_degree; d >= 0; --d) {
            const T c = coef[d + g.degree()] / g.coef.back();
            q[d] = c;
            for (unsigned int i = 0; i < g.degree(); ++i) {
                coef[d + i] -= c * g.coef[i];
            }
        }
        coef = q;
        normalize();
        return *this;
    }

    // divide (special case)
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
            return divide_slow(g);
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

    polynomial<T> integral() const {
        static_assert(!std::is_integral<T>::value, "polynomial() is not defined for integer coefficients.");
        std::vector<T> new_coef(coef.size() + 1);
        for (unsigned int i = 0; i < coef.size(); ++i) {
            new_coef[i + 1] = coef[i] / (i + 1);
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

// equality
template <class T>
bool operator==(const polynomial<T>& p, const polynomial<T>& q) {
    return p.coef == q.coef;
}

template <class T>
polynomial<T> multiply_many(std::vector<polynomial<T>> polys) {
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> q;
    for (unsigned int i = 0; i < polys.size(); ++i) {
        q.emplace(polys[i].degree(), i);
    }
    if (q.size() == 0) {
        return polynomial<T>(1);
    }
    while (q.size() > 1) {
        const int i = q.top().second; q.pop();
        const int j = q.top().second; q.pop();
        polys.emplace_back(polys[i]);
        polys.back() *= polys[j];
        q.emplace(polys.back().degree(), polys.size() - 1);
    }
    return polys[q.top().second];
}
}  // namespace amylase

#endif  // AMYLASE_POLYNOMIAL_HPP
