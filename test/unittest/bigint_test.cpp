#include <amylase/bigint>

#include <gtest/gtest.h>

using namespace amylase;
using ll = long long;
using ull = unsigned long long;

void assert_eq(const bigint& b, const long long l) {
    ASSERT_EQ(b.str_val(), std::to_string(l));
}

TEST(BigintTest, Serialize) {
    for (int p = -10; p <= 10; ++p) {
        assert_eq(bigint(p), p);
    }
    for (unsigned int p = 0; p < 60; ++p) {
        const ll q = 1ULL << p;
        assert_eq(bigint(q), q);
        assert_eq(bigint(-q), -q);
    }
}

TEST(BigintTest, SmallIntEquivalence) {
    const int bound = 100;
    for (int p = -bound; p <= bound; ++p) {
        bigint bp = p;
        for (int q = -bound; q <= bound; ++q) {
            bigint bq = q;
            assert_eq(bp + bq, p + q);
            assert_eq(bp - bq, p - q);
            assert_eq(bp * bq, p * q);
            ASSERT_EQ(bp < bq, p < q);
            ASSERT_EQ(bp > bq, p > q);
            ASSERT_EQ(bp <= bq, p <= q);
            ASSERT_EQ(bp >= bq, p >= q);
            ASSERT_EQ(bp == bq, p == q);
            ASSERT_EQ(bp != bq, p != q);

            if (q != 0) {
                assert_eq(bp / bq, p / q);
                assert_eq(bp % bq, p % q);
            }
        }
    }
}

TEST(BigintTest, LongLongEquivalence) {
    const int bound = 60;
    for (int _p = 0; _p <= bound; ++_p) {
        for (int ps = -1; ps <= 1; ps += 2) {
            long long p = ps * (1LL << _p);
            bigint bp = p;
            for (int _q = 0; _q <= bound; ++_q) {
                for (int qs = -1; qs <= 1; qs += 2) {
                    long long q = qs * (1LL << _q);
                    bigint bq = q;
                    assert_eq(bp + bq, p + q);
                    assert_eq(bp - bq, p - q);
                    if (_p + _q < bound) {
                        assert_eq(bp * bq, p * q);
                    }
                    ASSERT_EQ(bp < bq, p < q);
                    ASSERT_EQ(bp > bq, p > q);
                    ASSERT_EQ(bp <= bq, p <= q);
                    ASSERT_EQ(bp >= bq, p >= q);
                    ASSERT_EQ(bp == bq, p == q);
                    ASSERT_EQ(bp != bq, p != q);

                    if (q != 0) {
                        assert_eq(bp / bq, p / q);
                        assert_eq(bp % bq, p % q);
                    }
                }
            }
        }
    }
}
