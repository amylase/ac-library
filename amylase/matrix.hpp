#ifndef AMYLASE_MATRIX_HPP
#define AMYLASE_MATRIX_HPP 1

#include <vector>
#include <cassert>
#include <atcoder/modint>

namespace amylase {

template <class T>
struct matrix {
    std::vector<std::vector<T>> elem;

    explicit matrix(const int n): matrix(n, n) {}

    explicit matrix(const int r, const int c): matrix(std::vector<std::vector<T>>(r, std::vector<T>(c))) {}

    explicit matrix(const std::vector<std::vector<T>> _elem): elem(_elem) {
        assert(rows() > 0);
        const unsigned long long cols = elem[0].size();
        for (unsigned int i = 0; i < elem.size(); ++i) {
            assert(elem[i].size() == cols);
        }
    }

    static matrix<T> ident(const int n) {
        matrix<T> ret(n);
        for (int i = 0; i < n; ++i) {
            ret.elem[i][i] = 1;
        }
        return ret;
    }

    static matrix<T> row_vec(const std::vector<T>& vec) {
        return matrix<T>({vec});
    }

    static matrix<T> col_vec(const std::vector<T>& vec) {
        return row_vec(vec).transpose();
    }

    unsigned int rows() const {
        return (unsigned int) elem.size();
    }

    unsigned int cols() const {
        return (unsigned int) elem[0].size();
    }

    // concat
    matrix<T>& vstack(const matrix<T>& other) {
        assert(cols() == other.cols());
        for (unsigned int i = 0; i < other.rows(); ++i) {
            elem.emplace_back(other.elem[i]);
        }
        return *this;
    }

    matrix<T>& hstack(const matrix<T>& other) {
        assert(rows() == other.rows());
        for (unsigned int i = 0; i < rows(); ++i) {
            elem[i].insert(elem[i].end(), other.elem[i].begin(), other.elem[i].end());
        }
        return *this;
    }

    // add
    matrix<T>& operator+=(const matrix<T>& other) {
        assert(rows() == other.rows());
        assert(cols() == other.cols());
        for (unsigned int i = 0; i < rows(); ++i) {
            for (unsigned int j = 0; j < cols(); ++j) {
                elem[i][j] += other.elem[i][j];
            }
        }
        return *this;
    }

    matrix<T> operator+(const matrix<T>& other) const {
        return matrix<T>(*this) += other;
    }

    // scalar multiple
    matrix<T>& operator*=(const T other) {
        for (unsigned int i = 0; i < rows(); ++i) {
            for (unsigned int j = 0; j < cols(); ++j) {
                elem[i][j] *= other;
            }
        }
        return *this;
    }

    matrix<T> operator*(const T other) const {
        return matrix<T>(*this) *= other;
    }

    // unary negation
    matrix<T> operator-() const {
        return *this * -1;
    }

    // subtract
    matrix<T>& operator-=(const matrix<T>& other) {
        assert(rows() == other.rows());
        assert(cols() == other.cols());
        for (unsigned int i = 0; i < rows(); ++i) {
            for (unsigned int j = 0; j < cols(); ++j) {
                elem[i][j] -= other.elem[i][j];
            }
        }
        return *this;
    }

    matrix<T> operator-(const matrix<T>& other) const {
        return matrix<T>(*this) -= other;
    }

    // transpose
    matrix<T> transpose() const {
        matrix<T> ret(cols(), rows());
        for (unsigned int i = 0; i < rows(); ++i) {
            for (unsigned int j = 0; j < cols(); ++j) {
                ret.elem[j][i] = elem[i][j];
            }
        }
        return ret;
    }

    // matrix multiple
    matrix<T> operator*(const matrix<T>& other) const {
        assert(cols() == other.rows());
        matrix<T> other_t = other.transpose();
        std::vector<std::vector<T>> new_elem(rows(), std::vector<T>(other.cols()));
        for (unsigned int r = 0; r < rows(); ++r) {
            for (unsigned int c = 0; c < other.cols(); ++c) {
                for (unsigned int k = 0; k < cols(); ++k) {
                    new_elem[r][c] += elem[r][k] * other_t.elem[c][k];
                }
            }
        }
        return matrix<T>(new_elem);
    }

    matrix<T>& operator*=(const matrix<T>& other) {
        elem = (*this * other).elem;
        return *this;
    }

    // power
    matrix<T> pow(const int n) const {
        assert(rows() == cols());
        if (n == 0) {
            return ident(rows());
        }
        matrix<T> sq = pow(n / 2);
        if (n % 2 == 1) {
            return sq * sq * (*this);
        } else {
            return sq * sq;
        }
    }

    // gauss-jordan
    template <class mint = T, atcoder::internal::is_static_modint_t<mint>* = nullptr>
    matrix<mint> gauss_jordan() const {
        matrix<mint> result(*this);
        unsigned int next_row = 0;
        for (unsigned int c = 0; c < cols(); ++c) {
            unsigned int target = next_row;
            for (; target < rows(); ++target) {
                if (result.elem[target][c] != 0) {
                    break;
                }
            }
            if (target == rows()) {
                continue;
            }
            swap(result.elem[next_row], result.elem[target]);
            const mint inv = 1 / result.elem[next_row][c];
            for (unsigned int cc = 0; cc < cols(); ++cc) {
                result.elem[next_row][cc] *= inv;
            }

            for (unsigned int r = 0; r < rows(); ++r) {
                if (r == next_row) {
                    continue;
                }
                if (result.elem[r][c] == 0) {
                    continue;
                }
                const mint coef = -result.elem[r][c];
                for (unsigned int cc = 0; cc < cols(); ++cc) {
                    result.elem[r][cc] += coef * result.elem[next_row][cc];
                }
            }
            next_row += 1;
        }
        return result;
    }

    int rank() const {
        const matrix<T> g = gauss_jordan();
        int r = 0;
        for (unsigned int i = 0; i < rows(); ++i) {
            bool basis = false;
            for (unsigned int j = 0; j < cols(); ++j) {
                basis |= g.elem[i][j] != 0;
            }
            if (basis) {
                r += 1;
            }
        }
        return r;
    }
};

template <class T>
bool operator==(const matrix<T>& p, const matrix<T>& q) {
    return p.elem == q.elem;
}

}  // namespace amylase

#endif  // AMYLASE_MATRIX_HPP
