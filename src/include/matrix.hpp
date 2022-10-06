
/**
 * @file matrix.hpp
 * @brief 矩阵计算
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-09-07
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-07<td>Zone.N<td>迁移到 doxygen
 * </table>
 */

#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include "cfloat"
#include "cstring"
#include "iomanip"
#include "iostream"
#include "vector"

#include "log.hpp"
#include "vector.hpp"

/**
 * @brief 四阶矩阵
 * @tparam _T 矩阵元素类型
 */
template <class _T>
class matrix_t {
private:
    /// @brief  阶数
    static constexpr const uint8_t ORDER = 4;
    /// @brief 矩阵元素
    _T                             mat[ORDER][ORDER];

    /**
     * @brief 是否有非数值
     * @return true             有
     * @return false            无
     */
    bool                           HasNaNs(void) const;

    /**
     * @brief 递归求 n 阶行列式的值
     * @param  _order           阶数
     * @return _T               值
     */
    _T                             determ(const uint8_t _order) const;

    /**
     * @brief 代数余子式矩阵
     * @param  _row             行
     * @param  _col             列
     * @param  _order           当前阶
     * @return const matrix_t<_T>   结果
     */
    const matrix_t<_T> cofactor(const uint8_t _row, const uint8_t _col,
                                const uint8_t _order) const;

    /**
     * @brief 伴随矩阵
     * @return const matrix_t<_T>   结果
     */
    const matrix_t<_T> adjoint(void) const;

public:
    /**
     * @brief 构造函数，默认为单位矩阵
     */
    matrix_t(void);

    /**
     * @brief 构造函数
     * @param  _mat             另一个矩阵
     */
    explicit matrix_t(const matrix_t<_T>& _mat);

    /**
     * @brief 构造函数
     * @param  _arr             指针
     */
    explicit matrix_t(const _T* const _arr);

    /**
     * @brief 构造函数
     * @param  _arr             数组
     */
    explicit matrix_t(const _T _arr[ORDER][ORDER]);

    /**
     * @brief 构造函数，构造齐次坐标，多余位置补 0
     * @param  _v               四维向量
     */
    explicit matrix_t(const vector4_t<_T>& _v);

    /**
     * @brief 赋值
     * @param  _mat             另一个 matrix_t
     * @return matrix_t<_T>&    结果
     */
    matrix_t<_T>&      operator=(const matrix_t<_T>& _mat);

    /**
     * @brief 矩阵间加法
     * @param  _mat             另一个 matrix_t
     * @return const matrix_t<_T>   结果
     */
    const matrix_t<_T> operator+(const matrix_t<_T>& _mat) const;

    /**
     * @brief 矩阵自加
     * @param  _mat             另一个 matrix_t
     * @return matrix_t<_T>&    结果
     */
    matrix_t<_T>&      operator+=(const matrix_t<_T>& _mat);

    /**
     * @brief 矩阵间减法
     * @param  _mat             另一个 matrix_t
     * @return const matrix_t<_T>   结果
     */
    const matrix_t<_T> operator-(const matrix_t<_T>& _mat) const;

    /**
     * @brief 矩阵自减
     * @param  _mat             另一个 matrix_t
     * @return matrix_t<_T>&    结果
     */
    matrix_t<_T>&      operator-=(const matrix_t<_T>& _mat);

    /**
     * @brief 矩阵数乘
     * @tparam _U 数类型
     * @param  _v               数
     * @param  _mat             矩阵
     * @return const matrix_t<_T>   结果
     */
    template <class _U>
    friend const matrix_t<_T>
    operator*(const _U& _v, const matrix_t<_T>& _mat) {
        if (std::isnan(_v)) {
            throw std::invalid_argument(log("std::isnan(_v)"));
        }
        if (_mat.HasNaNs()) {
            throw std::invalid_argument(log("_mat.HasNaNs()"));
        }

        _T tmp[ORDER][ORDER] = { { 0 } };
        for (uint8_t i = 0; i < ORDER; i++) {
            for (uint8_t j = 0; j < ORDER; j++) {
                tmp[i][j] = _mat[i][j] * _v;
            }
        }
        return matrix_t<_T>(tmp);
    }

    /**
     * @brief 矩阵数乘
     * @tparam _U 数类型
     * @param  _mat             矩阵
     * @param  _v               数
     * @return const matrix_t<_T>   结果
     */
    template <class _U>
    friend const matrix_t<_T>
    operator*(const matrix_t<_T>& _mat, const _U& _v) {
        if (std::isnan(_v)) {
            throw std::invalid_argument(log("std::isnan(_v)"));
        }
        if (_mat.HasNaNs()) {
            throw std::invalid_argument(log("_mat.HasNaNs()"));
        }

        _T tmp[ORDER][ORDER] = { { 0 } };
        for (uint8_t i = 0; i < ORDER; i++) {
            for (uint8_t j = 0; j < ORDER; j++) {
                tmp[i][j] = _v * _mat[i][j];
            }
        }
        return matrix_t<_T>(tmp);
    }

    /**
     * @brief 行向量乘矩阵
     * @tparam _U 向量类型
     * @param  _v               向量
     * @param  _mat             矩阵
     * @return const vector4_t<_U>  结果
     */
    template <class _U>
    friend const vector4_t<_U>
    operator*(const vector4_t<_U>& _v, const matrix_t<_T>& _mat) {
        if (_v.HasNaNs()) {
            throw std::invalid_argument(log("_v.HasNaNs()"));
        }
        if (_mat.HasNaNs()) {
            throw std::invalid_argument(log("_mat.HasNaNs()"));
        }

        auto new_x = _v.x * _mat[0][0] + _v.y * _mat[1][0] + _v.z * _mat[2][0]
                   + _v.w * _mat[3][0];
        auto new_y = _v.x * _mat[0][1] + _v.y * _mat[1][1] + _v.z * _mat[2][1]
                   + _v.w * _mat[3][1];
        auto new_z = _v.x * _mat[0][2] + _v.y * _mat[1][2] + _v.z * _mat[2][2]
                   + _v.w * _mat[3][2];
        auto new_w = _v.x * _mat[0][3] + _v.y * _mat[1][3] + _v.z * _mat[2][3]
                   + _v.w * _mat[3][3];
        return vector4_t<_U>(new_x, new_y, new_z, new_w);
    }

    /**
     * @brief 矩阵乘列向量
     * @tparam _U 向量类型
     * @param  _mat             矩阵
     * @param  _v               向量
     * @return const vector4_t<_U>  结果
     */
    template <class _U>
    friend const vector4_t<_U>
    operator*(const matrix_t<_T>& _mat, const vector4_t<_U>& _v) {
        if (_mat.HasNaNs()) {
            throw std::invalid_argument(log("_mat.HasNaNs()"));
        }
        if (_v.HasNaNs()) {
            throw std::invalid_argument(log("_v.HasNaNs()"));
        }
        auto new_x = _v.x * _mat[0][0] + _v.y * _mat[0][1] + _v.z * _mat[0][2]
                   + _v.w * _mat[0][3];
        auto new_y = _v.x * _mat[1][0] + _v.y * _mat[1][1] + _v.z * _mat[1][2]
                   + _v.w * _mat[1][3];
        auto new_z = _v.x * _mat[2][0] + _v.y * _mat[2][1] + _v.z * _mat[2][2]
                   + _v.w * _mat[2][3];
        auto new_w = _v.x * _mat[3][0] + _v.y * _mat[3][1] + _v.z * _mat[3][2]
                   + _v.w * _mat[3][3];
        return vector4_t<_U>(new_x, new_y, new_z, new_w);
    }

    /**
     * @brief 矩阵间乘法
     * @param  _mat             另一个 matrix_t
     * @return const matrix_t<_T>   结果
     */
    const matrix_t<_T> operator*(const matrix_t<_T>& _mat) const;

    /**
     * @brief 矩阵数乘的自乘
     * @param  _v               数
     * @return matrix_t<_T>&    结果
     */
    matrix_t<_T>&      operator*=(const _T& _v);

    /**
     * @brief 行向量乘矩阵
     * @tparam _U 向量类型
     * @param  _v               向量
     * @param  _mat             矩阵
     * @return vector4_t<_U>&   结果
     */
    template <class _U>
    friend vector4_t<_U>&
    operator*=(vector4_t<_U>& _v, const matrix_t<_T>& _mat) {
        if (_v.HasNaNs()) {
            throw std::invalid_argument(log("_v.HasNaNs()"));
        }
        if (_mat.HasNaNs()) {
            throw std::invalid_argument(log("_mat.HasNaNs()"));
        }
        auto new_x = _v.x * _mat[0][0] + _v.y * _mat[1][0] + _v.z * _mat[2][0]
                   + _v.w * _mat[3][0];
        auto new_y = _v.x * _mat[0][1] + _v.y * _mat[1][1] + _v.z * _mat[2][1]
                   + _v.w * _mat[3][1];
        auto new_z = _v.x * _mat[0][2] + _v.y * _mat[1][2] + _v.z * _mat[2][2]
                   + _v.w * _mat[3][2];
        auto new_w = _v.x * _mat[0][3] + _v.y * _mat[1][3] + _v.z * _mat[2][3]
                   + _v.w * _mat[3][3];
        _v.x = new_x;
        _v.y = new_y;
        _v.z = new_z;
        _v.w = new_w;
        return _v;
    }

    /**
     * @brief 矩阵乘列向量
     * @tparam _U 向量类型
     * @param  _mat             矩阵
     * @param  _v               向量
     * @return vector4_t<_U>&   结果
     */
    template <class _U>
    friend vector4_t<_U>&
    operator*=(const matrix_t<_T>& _mat, vector4_t<_U>& _v) {
        if (_v.HasNaNs()) {
            throw std::invalid_argument(log("_v.HasNaNs()"));
        }
        auto new_x = _v.x * _mat[0][0] + _v.y * _mat[0][1] + _v.z * _mat[0][2]
                   + _v.w * _mat[0][3];
        auto new_y = _v.x * _mat[1][0] + _v.y * _mat[1][1] + _v.z * _mat[1][2]
                   + _v.w * _mat[1][3];
        auto new_z = _v.x * _mat[2][0] + _v.y * _mat[2][1] + _v.z * _mat[2][2]
                   + _v.w * _mat[2][3];
        auto new_w = _v.x * _mat[3][0] + _v.y * _mat[3][1] + _v.z * _mat[3][2]
                   + _v.w * _mat[3][3];
        _v.x = new_x;
        _v.y = new_y;
        _v.z = new_z;
        _v.w = new_w;
        return _v;
    }

    /**
     * @brief 矩阵间自乘
     * @param  _mat             另一个 matrix_t
     * @return matrix_t<_T>&    结果
     */
    matrix_t<_T>&      operator*=(const matrix_t<_T>& _mat);

    /**
     * @brief 矩阵相等
     * @param  _mat             另一个 matrix_t
     * @return true             相等
     * @return false            不等
     */
    bool               operator==(const matrix_t<_T>& _mat) const;

    /**
     * @brief 矩阵不等
     * @param  _mat             另一个 matrix_t
     * @return true             不等
     * @return false            相等
     */
    bool               operator!=(const matrix_t<_T>& _mat) const;

    /**
     * @brief 下标重载
     * @param  _idx             行
     * @return _T*              行指针
     * @note    注意不要越界访问
     */
    _T*                operator[](const uint8_t _idx);

    /**
     * @brief 下标重载
     * @param  _idx             行
     * @return const _T*        行指针
     */
    const _T*          operator[](const uint8_t _idx) const;

    /**
     * @brief 矩阵转置
     * @return const matrix_t<_T>   转置矩阵
     */
    const matrix_t<_T> transpose(void) const;

    /**
     * @brief 逆矩阵
     * @return const matrix_t<_T>       逆矩阵
     * @see https://www.geeksforgeeks.org/adjoint-inverse-matrix/
     */
    const matrix_t<_T> inverse(void) const;

    /**
     * @brief 平移矩阵
     * @param  _x               x 方向变换
     * @param  _y               y 方向变换
     * @param  _z               z 方向变换
     * @return const matrix_t<_T>   构造好的平移矩阵
     * @note    先旋转再平移
     */
    const matrix_t<_T> translate(const _T _x, const _T _y, const _T _z) const;

    /**
     * @brief 缩放矩阵
     * @param  _scale           缩放倍数
     * @return const matrix_t<_T>   构造好的旋转矩阵
     */
    const matrix_t<_T> scale(const _T _scale) const;

    /**
     * @brief 缩放矩阵
     * @param  _x               x 方向缩放倍数
     * @param  _y               y 方向缩放倍数
     * @param  _z               z 方向缩放倍数
     * @return const matrix_t<_T>   构造好的旋转矩阵
     */
    const matrix_t<_T> scale(const _T _x, const _T _y, const _T _z) const;

    /**
     * @brief 旋转矩阵
     * @param  _x               旋转中心的 x 坐标
     * @param  _y               旋转中心的 y 坐标
     * @param  _z               旋转中心的 z 坐标
     * @param  _angle           要旋转的弧度
     * @return const matrix_t<_T>   构造好的旋转矩阵
     * @see http://docs.gl/gl2/glRotate
     * @note    先旋转再平移
     */
    const matrix_t<_T>
    rotate(const _T _x, const _T _y, const _T _z, const float _angle) const;

    /**
     * @brief 角度转换为弧度
     * @param  _deg             角度
     * @return float            弧度
     */
    static float RAD(const float _deg);

    /**
     * @brief 弧度转换为角度
     * @param  _rad             弧度
     * @return float            角度
     */
    static float DEG(const float _rad);

    friend std::ostream&
    operator<<(std::ostream& _os, const matrix_t<_T>& _mat) {
        _os << "[";
        for (uint8_t i = 0; i < matrix_t<_T>::ORDER; i++) {
            if (i != 0) {
                _os << "\n";
                _os << " ";
            }
            for (uint8_t j = 0; j < matrix_t<_T>::ORDER; j++) {
                _os << std::setw(7) << std::setprecision(8) << _mat[i][j];
                if (j != matrix_t<_T>::ORDER - 1) {
                    _os << " ";
                }
            }
        }
        _os << std::setw(4) << "]";
        return _os;
    }
};

template <class _T>
bool matrix_t<_T>::HasNaNs(void) const {
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            if (std::isnan(mat[i][j]) == true) {
                return true;
            }
        }
    }
    return false;
}

template <class _T>
_T matrix_t<_T>::determ(const uint8_t _order) const {
    // 递归返回条件
    if (_order == 1) {
        return mat[0][0];
    }

    _T  res  = 0;
    // 当前正负
    int sign = 1;
    // 计算 mat[0][i] 的代数余子式
    for (uint8_t i = 0; i < _order; i++) {
        res  += sign * mat[0][i] * cofactor(0, i, _order).determ(_order - 1);
        // 符号取反
        sign = -sign;
    }
    return res;
}

template <class _T>
const matrix_t<_T>
matrix_t<_T>::cofactor(const uint8_t _row, const uint8_t _col,
                       const uint8_t _order) const {
    _T   tmp[ORDER][ORDER] = { { 0 } };
    auto row_idx           = 0;
    auto col_idx           = 0;
    for (uint8_t i = 0; i < _order; i++) {
        for (uint8_t j = 0; j < _order; j++) {
            if (i != _row && j != _col) {
                tmp[row_idx][col_idx++] = mat[i][j];
                // 换行
                if (col_idx == _order - 1) {
                    col_idx = 0;
                    row_idx++;
                }
            }
        }
    }
    return matrix_t<_T>(tmp);
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::adjoint(void) const {
    _T  tmp[ORDER][ORDER] = { { 0 } };
    // 当前正负
    int sign              = 1;
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            // 判断当前符号，行列索引之和为偶数时为正
            sign      = ((i + j) % 2 == 0) ? 1 : -1;
            // 计算代数余子式矩阵并转置
            tmp[j][i] = (sign) * (cofactor(i, j, ORDER).determ(ORDER - 1));
        }
    }
    return matrix_t<_T>(tmp);
}

template <class _T>
matrix_t<_T>::matrix_t(void) {
    memset(mat, 0, ORDER * ORDER * sizeof(_T));
    for (uint8_t i = 0; i < ORDER; i++) {
        mat[i][i] = 1;
    }
    return;
}

template <class _T>
matrix_t<_T>::matrix_t(const matrix_t<_T>& _mat) {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    memcpy(mat, _mat.mat, ORDER * ORDER * sizeof(_T));
    return;
}

template <class _T>
matrix_t<_T>::matrix_t(const _T* const _arr) {
    if (_arr == nullptr) {
        throw std::invalid_argument(log("_arr == nullptr"));
    }
    for (auto i = 0; i < ORDER * ORDER; i++) {
        if (std::isnan(_arr[i])) {
            throw std::invalid_argument(log("std::isnan(_arr[i])"));
        }
    }
    memcpy(mat, _arr, ORDER * ORDER * sizeof(_T));
    return;
}

template <class _T>
matrix_t<_T>::matrix_t(const _T _arr[ORDER][ORDER]) {
    if (_arr == nullptr) {
        throw std::invalid_argument(log("_arr == nullptr"));
    }
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            if (std::isnan(_arr[i][j])) {
                throw std::invalid_argument(log("std::isnan(_arr[i][j])"));
            }
        }
    }
    memcpy(mat, _arr, ORDER * ORDER * sizeof(_T));
    return;
}

template <class _T>
matrix_t<_T>::matrix_t(const vector4_t<_T>& _v) {
    if (_v.HasNaNs()) {
        throw std::invalid_argument(log("_v.HasNaNs()"));
    }
    memset(mat, 0, ORDER * ORDER * sizeof(_T));
    mat[0][0] = _v.x;
    mat[1][1] = _v.y;
    mat[2][2] = _v.z;
    mat[3][3] = _v.w;
    return;
}

template <class _T>
matrix_t<_T>& matrix_t<_T>::operator=(const matrix_t<_T>& _mat) {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    if (this == &_mat) {
        throw std::runtime_error(log("this == &_mat"));
    }
    memcpy(mat, _mat.mat, ORDER * ORDER * sizeof(_T));
    return *this;
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::operator+(const matrix_t<_T>& _mat) const {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    _T tmp[ORDER][ORDER] = { { 0 } };
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            tmp[i][j] = mat[i][j] + _mat[i][j];
        }
    }
    return matrix_t<_T>(tmp);
}

template <class _T>
matrix_t<_T>& matrix_t<_T>::operator+=(const matrix_t<_T>& _mat) {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            mat[i][j] += _mat[i][j];
        }
    }
    return *this;
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::operator-(const matrix_t<_T>& _mat) const {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    _T tmp[ORDER][ORDER] = { { 0 } };
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            tmp[i][j] = mat[i][j] - _mat[i][j];
        }
    }
    return matrix_t<_T>(tmp);
}

template <class _T>
matrix_t<_T>& matrix_t<_T>::operator-=(const matrix_t<_T>& _mat) {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            mat[i][j] -= _mat[i][j];
        }
    }
    return *this;
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::operator*(const matrix_t<_T>& _mat) const {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    _T tmp[ORDER][ORDER] = { { 0 } };
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            for (uint8_t k = 0; k < ORDER; k++) {
                tmp[i][j] += mat[i][k] * _mat[k][j];
            }
        }
    }
    return matrix_t<_T>(tmp);
}

template <class _T>
matrix_t<_T>& matrix_t<_T>::operator*=(const _T& _v) {
    if (std::isnan(_v)) {
        throw std::invalid_argument(log("std::isnan(_v)"));
    }
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            mat[i][j] *= _v;
        }
    }
    return *this;
}

template <class _T>
matrix_t<_T>& matrix_t<_T>::operator*=(const matrix_t<_T>& _mat) {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    _T tmp[ORDER][ORDER] = { { 0 } };
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            for (uint8_t k = 0; k < ORDER; k++) {
                tmp[i][j] += mat[i][k] * _mat[k][j];
            }
        }
    }
    memcpy(mat, tmp, ORDER * ORDER * sizeof(_T));
    return *this;
}

template <class _T>
bool matrix_t<_T>::operator==(const matrix_t<_T>& _mat) const {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            if (std::abs(mat[i][j] - _mat[i][j])
                > std::numeric_limits<_T>::epsilon()) {
                return false;
            }
        }
    }
    return true;
}

template <class _T>
bool matrix_t<_T>::operator!=(const matrix_t<_T>& _mat) const {
    if (_mat.HasNaNs()) {
        throw std::invalid_argument(log("_mat.HasNaNs()"));
    }
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            if (std::abs(mat[i][j] - _mat[i][j])
                > std::numeric_limits<_T>::epsilon()) {
                return true;
            }
        }
    }
    return false;
}

template <class _T>
_T* matrix_t<_T>::operator[](const uint8_t _idx) {
    if (_idx > ORDER) {
        throw std::invalid_argument(log("_idx > ORDER"));
    }
    return mat[_idx];
}

template <class _T>
const _T* matrix_t<_T>::operator[](const uint8_t _idx) const {
    if (_idx > ORDER) {
        throw std::invalid_argument(log("_idx > ORDER"));
    }
    return mat[_idx];
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::transpose(void) const {
    _T tmp[ORDER][ORDER] = { { 0 } };
    for (uint8_t i = 0; i < ORDER; i++) {
        for (uint8_t j = 0; j < ORDER; j++) {
            tmp[j][i] = mat[i][j];
        }
    }
    return matrix_t<_T>(tmp);
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::inverse(void) const {
    if (HasNaNs()) {
        throw std::invalid_argument(log("HasNaNs()"));
    }
    // 计算行列式
    _T det = determ(ORDER);
    if (std::abs(det) <= std::numeric_limits<_T>::epsilon()) {
        throw std::runtime_error(
          log("std::abs(det) <= std::numeric_limits<_T>::epsilon(), Singular "
              "matrix, can't find its inverse"));
    }

    // 计算伴随矩阵
    auto adj = adjoint();

    // 逆矩阵=伴随矩阵/行列式
    auto tmp = adj * (1 / det);

    return matrix_t<_T>(tmp);
}

template <class _T>
const matrix_t<_T>
matrix_t<_T>::translate(const _T _x, const _T _y, const _T _z) const {
    if (std::isnan(_x) || std::isnan(_y) || std::isnan(_z)) {
        throw std::invalid_argument(
          log("std::isnan(_x) || std::isnan(_y) || std::isnan(_z)"));
    }
    matrix_t<_T> tmp;
    tmp.mat[0][3] = _x;
    tmp.mat[1][3] = _y;
    tmp.mat[2][3] = _z;
    tmp.mat[3][3] = 1;
    return tmp * *this;
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::scale(const _T _scale) const {
    if (std::isnan(_scale)) {
        throw std::invalid_argument(log("std::isnan(_scale)"));
    }
    matrix_t<_T> tmp;
    tmp.mat[0][0] = _scale;
    tmp.mat[1][1] = _scale;
    tmp.mat[2][2] = _scale;
    tmp.mat[3][3] = 1;
    return tmp * *this;
}

template <class _T>
const matrix_t<_T>
matrix_t<_T>::scale(const _T _x, const _T _y, const _T _z) const {
    if (std::isnan(_x) || std::isnan(_y) || std::isnan(_z)) {
        throw std::invalid_argument(
          log("std::isnan(_x) || std::isnan(_y) || std::isnan(_z)"));
    }
    matrix_t<_T> tmp;
    tmp.mat[0][0] = _x;
    tmp.mat[1][1] = _y;
    tmp.mat[2][2] = _z;
    tmp.mat[3][3] = 1;
    return tmp * *this;
}

template <class _T>
const matrix_t<_T> matrix_t<_T>::rotate(const _T _x, const _T _y, const _T _z,
                                        const float _angle) const {
    if (std::isnan(_x) || std::isnan(_y) || std::isnan(_z)
        || std::isnan(_angle)) {
        throw std::invalid_argument(
          log("std::isnan(_x) || std::isnan(_y) || std::isnan(_z)|| "
              "std::isnan(_angle)"));
    }
    auto         n = vector4_t(_x, _y, _z).normalize();
    auto         c = std::cos(_angle);
    auto         s = std::sin(_angle);

    matrix_t<_T> tmp;
    tmp.mat[0][0] = n.x * n.x * (1 - c) + c;
    tmp.mat[0][1] = n.y * n.x * (1 - c) - s * n.z;
    tmp.mat[0][2] = n.z * n.x * (1 - c) + s * n.y;

    tmp.mat[1][0] = n.x * n.y * (1 - c) + s * n.z;
    tmp.mat[1][1] = n.y * n.y * (1 - c) + c;
    tmp.mat[1][2] = n.z * n.y * (1 - c) - s * n.x;

    tmp.mat[2][0] = n.x * n.z * (1 - c) - s * n.y;
    tmp.mat[2][1] = n.y * n.z * (1 - c) + s * n.x;
    tmp.mat[2][2] = n.z * n.z * (1 - c) + c;

    return tmp * *this;
}

template <class _T>
float matrix_t<_T>::RAD(const float _deg) {
    if (std::isnan(_deg)) {
        throw std::invalid_argument(log("std::isnan(_deg)"));
    }
    return ((M_PI / 180) * (_deg));
}

template <class _T>
float matrix_t<_T>::DEG(const float _rad) {
    if (std::isnan(_rad)) {
        throw std::invalid_argument(log("std::isnan(_rad)"));
    }
    return ((180 / M_PI) * (_rad));
}

typedef matrix_t<float> matrix4f_t;

#endif /* _MATRIX_HPP_ */
