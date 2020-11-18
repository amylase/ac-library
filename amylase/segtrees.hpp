#ifndef AMYLASE_SEGTREES_HPP
#define AMYLASE_SEGTREES_HPP 1

#include <algorithm>
#include <limits>
#include <atcoder/segtree>
#include <atcoder/lazysegtree>

namespace amylase {

    // functions
    template<class T> T min(const T x, const T y) { return std::min(x, y); }
    template<class T> T max(const T x, const T y) { return std::max(x, y); }
    template<class T> T add(const T x, const T y) { return x + y; }
    template<class T> T zero() { return 0; }

    // segtrees: `prod`_segtree
    template<class T> using min_segtree = atcoder::segtree<T, min<T>, std::numeric_limits<T>::max>;
    template<class T> using max_segtree = atcoder::segtree<T, max<T>, std::numeric_limits<T>::lowest>;
    template<class T> using sum_segtree = atcoder::segtree<T, add<T>, zero<T>>;

    // lazy segtrees: `prod`_`func`_segtree
    template<class T> using min_min_segtree = atcoder::lazy_segtree<T, min<T>, std::numeric_limits<T>::max, T, min<T>, min<T>, std::numeric_limits<T>::max>;
    template<class T> using min_sum_segtree = atcoder::lazy_segtree<T, min<T>, std::numeric_limits<T>::max, T, add<T>, add<T>, zero<T>>;
    template<class T> using max_max_segtree = atcoder::lazy_segtree<T, max<T>, std::numeric_limits<T>::lowest, T, max<T>, max<T>, std::numeric_limits<T>::lowest>;
    template<class T> using max_sum_segtree = atcoder::lazy_segtree<T, max<T>, std::numeric_limits<T>::lowest, T, add<T>, add<T>, zero<T>>;
    template<class T> using sum_sum_segtree = atcoder::lazy_segtree<T, add<T>, zero<T>, T, add<T>, add<T>, zero<T>>;

    // NOTE: `sum_max_segtree` and `sum_min_segtree` is not available because straightforward approach does not work.
    // You can implement these operations if the array is monotone (not necessarily strict). Sketch is as follows.
    // Mapper is `overwrite`. You can determine which element should be overwritten when range-max is queried (Monotonicity is exploited here).
    // Each element manages range_sum, range_max, size. Composition and Production are obvious by definitions.
    // Example: https://codeforces.com/problemset/submission/1439/98821386

}  // namespace amylase

#endif  // AMYLASE_SEGTREES_HPP
