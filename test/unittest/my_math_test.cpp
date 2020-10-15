#include <amylase/math>
#include <numeric>

#include <gtest/gtest.h>

using ll = long long;

bool is_prime_naive(ll n) {
    assert(0 <= n && n <= std::numeric_limits<int>::max());
    if (n == 0 || n == 1) return false;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

TEST(MyMathTest, IsPrime) {
    for (long long i = 0; i < 10000; ++i) {
        ASSERT_EQ(is_prime_naive(i), amylase::is_prime(i));
    }
    ASSERT_EQ(is_prime_naive(std::numeric_limits<int>::max()), amylase::is_prime(std::numeric_limits<int>::max()));
    long long testcases[10] = {
        4559258600391305197,
        4870395676773890257,
        5543343419754101539,
        2696910492149275857,
        8444122399798758311,
        5538547194568284803,
        5828352936117750187,
        8921160186595818731,
        3423153523301153381,
        2110440756486307547
    };
    bool expected[10] = {true, true, true,  false, true,
                         true, true, false, false, true};
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(expected[i], amylase::is_prime(testcases[i]));
    }
}

TEST(MyMathTest, Factor) {
    for (long long i = 1; i < 10000; ++i) {
        const auto fact = amylase::factor(i);
        long long prod = 1;
        for (const auto& p : fact) {
            prod *= p;
            ASSERT_TRUE(amylase::is_prime(p));
        }
        ASSERT_EQ(prod, i);
    }
}
