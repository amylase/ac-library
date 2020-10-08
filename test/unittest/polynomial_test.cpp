#include <amylase/polynomial>
#include <atcoder/modint>
#include <gtest/gtest.h>

using li = long long;
using mint = atcoder::modint998244353;
using intpoly = amylase::polynomial<li>;
using modpoly = amylase::polynomial<mint>;
using floatpoly = amylase::polynomial<double>;

modpoly multiply_naive(const modpoly& p, const modpoly& q) {
    std::vector<mint> coef(p.degree() + q.degree() + 1);
    for (unsigned int i = 0; i < p.coef.size(); ++i) {
        for (unsigned int j = 0; j < q.coef.size(); ++j) {
            coef[i + j] += p.coef[i] * q.coef[j];
        }
    }
    return modpoly(coef);
}

modpoly testcase(const unsigned int degree) {
    std::vector<mint> coef(degree + 1);
    for (unsigned int i = 0; i < coef.size(); ++i) {
        coef[i] = i + 1;
    }
    return modpoly(coef);
}

TEST(PolynomialTest, Add) {
    const intpoly p({1, 2});
    const intpoly q({3, 4, 5});
    const intpoly r = p + q;
    const intpoly rr = q + p;
    ASSERT_EQ(r, intpoly({4, 6, 5}));
    ASSERT_EQ(rr, intpoly({4, 6, 5}));
}

TEST(PolynomialTest, Subtract) {
    const intpoly p({1, 2, 5});
    const intpoly q({3, 4, 5});
    const intpoly r = p - q;

    ASSERT_EQ(r, intpoly({-2, -2}));
}

TEST(PolynomialTest, MultiplyMod) {
    const modpoly p({1, 2, 5});
    const modpoly q({3, 4, 5});
    const modpoly r = p * q;

    ASSERT_EQ(r, modpoly({3, 10, 28, 30, 25}));

    for (int p_deg = 0; p_deg < 100; ++p_deg) {
        const modpoly pp = testcase(p_deg);
        for (int q_deg = 0; q_deg < 100; ++q_deg) {
            const modpoly qq = testcase(q_deg);
            const modpoly actual = pp * qq;
            const modpoly expected = multiply_naive(pp, qq);
            ASSERT_EQ(pp * qq, multiply_naive(pp, qq));
        }
    }
}

TEST(PolynomialTest, MultiplyInt) {
    const intpoly p({1, 2, 5});
    const intpoly q({3, 4, 5});
    const intpoly r = p * q;

    ASSERT_EQ(r, intpoly({3, 10, 28, 30, 25}));
}

TEST(PolynomialTest, Divide) {
    const modpoly pp({0, 0, 0, 1});
    const modpoly qq({-1, 1});
    const modpoly rr = pp / qq;
    const modpoly ss = pp % qq;
    ASSERT_EQ(rr, modpoly({1, 1, 1}));
    ASSERT_EQ(ss, modpoly(1));

    const auto test = [](const int p_deg, const int q_deg) {
      const modpoly p = testcase(p_deg);
      const modpoly q = testcase(q_deg);
      const modpoly x = p / q;
      const modpoly y = p % q;
      ASSERT_EQ(p, x * q + y);
    };

    for (int p_deg = 0; p_deg < 100; ++p_deg) {
        for (int q_deg = 0; q_deg < 100; ++q_deg) {
            test(p_deg, q_deg);
        }
    }

    test(1'000'000, 2);
    test(1'000'000, 999'999);
    test(1'000, 500);
}

TEST(PolynomialTest, Substitute) {
    const intpoly p({1, 2, 5});

    ASSERT_EQ(25, p.substitute(2));
}

TEST(PolynomialTest, Derivative) {
    const intpoly p({1, 2, 5});

    ASSERT_EQ(p.derivative(), intpoly({2, 10}));
}

TEST(PolynomialTest, ModInt) {
    const modpoly p({1, 2});
    const modpoly q({3, 4});
    const modpoly r = p - q;

    ASSERT_EQ(r, modpoly({-2, -2}));
}

TEST(PolynomialTest, Float) {
    const floatpoly p({1.5, 2.5});
    const floatpoly q({3.5, 4.5});
    const floatpoly r = p - q;

    ASSERT_EQ(r, floatpoly({-2, -2}));
}