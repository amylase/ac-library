#include <amylase/combinatorics>

#include <atcoder/modint>
#include <amylase/mod2>
#include <vector>

#include <gtest/gtest.h>


template <class mint>
void test_struct(amylase::combinatorics<mint> c) {
    const int n = 300;

    mint fact = 1;
    for (int i = 1; i < n; ++i) {
        fact *= i;
        ASSERT_EQ(fact, c.factorial(i));
    }

    std::vector<std::vector<mint>> naive_combination(n, std::vector<mint>(n));
    naive_combination[0][0] = 1;
    for (int r = 1; r < n; ++r) {
        for (int k = 0; k < n; ++k) {
            if (k < 0 || k > r) {
                naive_combination[r][k] = 0;
            } else if (k == 0 || k == r) {
                naive_combination[r][k] = 1;
            } else {
                naive_combination[r][k] = naive_combination[r - 1][k - 1] + naive_combination[r - 1][k];
            }
            ASSERT_EQ(naive_combination[r][k], c.combination(r, k));
        }
    }

    for (int r = 0; r < n; ++r) {
        for (int k = 0; k < n; ++k) {
            if (k > r) {
                ASSERT_EQ(0, c.permutation(r, k));
            } else {
                mint expected = 1;
                for (int i = 0; i < k; ++i) {
                    expected *= r - i;
                }
                ASSERT_EQ(expected, c.permutation(r, k));
            }
        }
    }
}

TEST(CombinatoricsTest, Test1e9p7) {
    amylase::combinatorics<atcoder::modint1000000007> c;
    test_struct(c);
}

TEST(CombinatoricsTest, TestNTTMod) {
    amylase::combinatorics<atcoder::modint998244353> c;
    test_struct(c);
}

TEST(CombinatoricsTest, TestMod2) {
    amylase::combinatorics<atcoder::static_modint<2>> c;
    test_struct(c);
}

TEST(CombinatoricsTest, TestMyMod2) {
    amylase::combinatorics<amylase::mod2> c;
    test_struct(c);
}
