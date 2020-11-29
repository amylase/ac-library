#include <amylase/rational>
#include <atcoder/modint>

#include <gtest/gtest.h>

using namespace amylase;
using mint = atcoder::modint998244353;

int gcd(const int x, const int y) {
    if (x == 0) {
        return y;
    }
    return gcd(y % x, x);
}

mint to_mint(const bigint& r) {
    mint ret = 0;
    for (const auto v : r.data) {
        ret *= amylase::_size;
        ret += v;
    }
    if (r.sign == -1) {
        ret *= -1;
    }
    return ret;
}

mint to_mint(const rational& r) {
    return to_mint(r.dividend) / to_mint(r.divisor);
}

void assert_eq(const rational& r, const mint m) {
    ASSERT_EQ(to_mint(r), m);
}

TEST(RationalTest, Serialize) {
    for (int p = -10; p <= 10; ++p) {
        ASSERT_EQ(rational(p).str_val(), std::to_string(p));
    }
    for (int p = -50; p <= 50; ++p) {
        for (int q = 1; q <= 20; ++q) {
            int g = gcd(abs(p), q);
            std::string expected;
            if (q / g == 1) {
                expected = std::to_string(p / g);
            } else {
                expected = std::to_string(p / g) + "/" + std::to_string(q / g);
            }
            ASSERT_EQ(rational(p, q).str_val(), expected);
        }
    }
}

TEST(RationalTest, ModIntEquivalence) {
    const int bound = 5;
    for (int p = -bound; p <= bound; ++p) {
        for (int q = 1; q <= bound; ++q) {
            mint mpq = mint(p) / mint(q);
            rational rpq = rational(p, q);
            for (int r = -bound; r <= bound; ++r) {
                for (int s = 1; s <= bound; ++s) {
                    mint mrs = mint(r) / mint(s);
                    rational rrs = rational(r, s);

                    assert_eq(rpq + rrs, mpq + mrs);
                    assert_eq(rpq - rrs, mpq - mrs);
                    assert_eq(rpq * rrs, mpq * mrs);
                    if (r != 0) {
                        assert_eq(rpq / rrs, mpq / mrs);
                    }
                    ASSERT_EQ(rpq == rrs, mpq == mrs);
                    ASSERT_EQ(rpq != rrs, mpq != mrs);
                }
            }
        }
    }
}
