#include <amylase/matrix>
#include <atcoder/modint>
#include <gtest/gtest.h>

using li = long long;
using mint = atcoder::modint1000000007;
using intmat = amylase::matrix<li>;
using modmat = amylase::matrix<mint>;

TEST(MatrixTest, Vstack) {
    intmat p = intmat::row_vec({1, 2});
    intmat q = intmat::row_vec({3, 4});
    p.vstack(q);
    ASSERT_EQ(p, intmat({{1, 2}, {3, 4}}));
}

TEST(MatrixTest, Hstack) {
    intmat p = intmat::row_vec({1, 2});
    intmat q = intmat::row_vec({3, 4});
    p.hstack(q);
    ASSERT_EQ(p, intmat({{1, 2, 3, 4}}));
}

TEST(MatrixTest, Add) {
    intmat p = intmat::row_vec({1, 2});
    intmat q = intmat::row_vec({3, 4});
    ASSERT_EQ(p + q, intmat::row_vec({4, 6}));
}

TEST(MatrixTest, Subtract) {
    intmat p = intmat::row_vec({1, 2});
    intmat q = intmat::row_vec({3, 4});
    ASSERT_EQ(p - q, intmat::row_vec({-2, -2}));
}

TEST(MatrixTest, Multiply) {
    intmat p = intmat::row_vec({1, 2});
    intmat q = intmat::col_vec({3, 4});
    ASSERT_EQ(p * q, intmat::ident(1) * 11);
    ASSERT_EQ(q * p, intmat({{3, 6}, {4, 8}}));

}

TEST(MatrixTest, Power) {
    intmat p({{1, 2}, {3, 4}});
    ASSERT_EQ(p.pow(5), intmat({{1069, 1558}, {2337, 3406}}));
}

TEST(MatrixTest, Rank) {
    ASSERT_EQ(modmat({{1, 2}, {3, 4}}).rank(), 2);
    ASSERT_EQ(modmat({{1, 2}, {1, 2}}).rank(), 1);
    ASSERT_EQ(modmat({{1, 1}, {2, 2}}).rank(), 1);
    ASSERT_EQ(modmat({{0, 0}, {0, 0}}).rank(), 0);
}