#ifndef AMYLASE_COMBINATORICS_HPP
#define AMYLASE_COMBINATORICS_HPP 1

#include <vector>
#include <atcoder/modint>

namespace amylase {
    template <class T>
    struct combinatorics {
        static_assert(atcoder::internal::is_static_modint<T>::value, "combinatorics class only accepts atcoder::static_modint as a type parameter.");

        std::vector<T> nonzerofact_memo;
        std::vector<bool> invfact_contains;
        std::vector<T> invfact_memo;
        std::vector<long long> ordfact_memo;
        combinatorics(): nonzerofact_memo(1, 1), ordfact_memo(1, 0) {}

        T factorial(const long long n) {
            assert(n >= 0);
            if (n >= T::mod()) {
                return 0;
            }
            return nonzerofact(n);
        }

        T nonzerofact(const long long n) {
            assert(n >= 0);
            while (nonzerofact_memo.size() < (unsigned long long) n + 1) {
                unsigned long long x = nonzerofact_memo.size();
                while (x % T::mod() == 0) {
                    x /= T::mod();
                }
                T next = nonzerofact_memo.back() * x;
                nonzerofact_memo.emplace_back(next);
            }
            return nonzerofact_memo[n];
        }

        T invfact(const long long n) {
            assert(n >= 0);
            if (invfact_memo.size() <= (unsigned long long) n) {
                invfact_memo.resize(n + 1);
                invfact_contains.resize(n + 1, false);
            }
            if (!invfact_contains[n]) {
                invfact_memo[n] = 1 / nonzerofact(n);
            }
            return invfact_memo[n];
        }

        long long ordfact(const long long n) {
            assert(n >= 0);
            while (ordfact_memo.size() <= (unsigned long long) n) {
                long long x = ordfact_memo.size();
                long long ord_n = 0;
                while (x % T::mod() == 0) {
                    x /= T::mod();
                    ord_n += 1;
                }
                ordfact_memo.emplace_back(ordfact_memo.back() + ord_n);
            }
            return ordfact_memo[n];
        }

        T combination(const long long n, const long long k) {
            if (k < 0 || n < k) {
                return 0;
            }
            if (ordfact(n) > ordfact(k) + ordfact(n - k)) {
                return 0;
            }
            return nonzerofact(n) * invfact(k) * invfact(n - k);
        }

        T permutation(const long long n, const long long k) {
            if (k < 0 || n < k) {
                return 0;
            }
            if (ordfact(n) > ordfact(n - k)) {
                return 0;
            }
            return nonzerofact(n) * invfact(n - k);
        }
    };
}  // namespace amylase

#endif  // AMYLASE_COMBINATORICS_HPP
