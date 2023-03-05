
/**
 * @file color.h
 * @brief 颜色抽象
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-1-02
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-1-02<td>Zone.N<td>创建文件
 * </table>
 */

#ifndef SIMPLERENDER_COLOR_H
#define SIMPLERENDER_COLOR_H

#include "cstdint"
#include "iostream"

#include "vector.hpp"

/**
 * @brief 32 位颜色 RGBA
 * @note 不要进行会影响内存的修改
 */
class color_t {
private:
    /// @brief 颜色深度
    static constexpr const uint8_t DEPTH = 4;

    /// @brief 每像素字节数
    static constexpr const size_t  BPP   = sizeof(uint8_t) * DEPTH;

    /// @brief 颜色数据，rgba
    uint8_t                        channel_r;
    uint8_t                        channel_g;
    uint8_t                        channel_b;
    uint8_t                        channel_a;

public:
    static color_t WHITE;
    static color_t BLACK;
    static color_t RED;
    static color_t GREEN;
    static color_t BLUE;

    enum color_idx_t {
        COLOR_IDX_R = 0,
        COLOR_IDX_G = 1,
        COLOR_IDX_B = 2,
        COLOR_IDX_A = 3,
    };

    /**
     * @brief 构造函数
     */
    color_t(void);

    /**
     * @brief 构造函数
     * @param  _data            颜色数据
     */
    explicit color_t(uint32_t _data);

    /**
     * @brief 构造函数
     * @param  _r               红色
     * @param  _g               绿色
     * @param  _b               蓝色
     * @param  _a               alpha
     * @param  _color_order     颜色顺序，默认为 RGBA
     */
    explicit color_t(uint8_t _r, uint8_t _g, uint8_t _b,
                     uint8_t _a = std::numeric_limits<uint8_t>::max());

    /**
     * @brief 构造函数，从 [0, 1] 构建
     * @param  _r               红色
     * @param  _g               绿色
     * @param  _b               蓝色
     * @param  _a               alpha
     */
    explicit color_t(float _r, float _g, float _b,
                     uint8_t _a = std::numeric_limits<uint8_t>::max());

    /**
     * @brief 构造函数
     * @param  _color  另一个 color_t
     */
    color_t(const color_t& _color);

    /**
     * @brief 析构函数
     */
    ~color_t(void);

    /**
     * @brief = 重载
     * @param  _color  另一个 color_t
     * @return color_t& 结果
     */
    color_t&             operator=(const color_t& _color);

    /// @todo 注释

    color_t              operator*(float _f) const;

    color_t              operator*(const vector4f_t& _vector4f) const;

    color_t&             operator*=(float _f);

    color_t&             operator*=(const vector4f_t& _vector4f);

    /**
     * @brief 下标重载
     * @param  _idx             索引
     * @return uint8_t&         对应颜色
     */
    uint8_t&             operator[](uint8_t _idx);

    /**
     * @brief 下标重载
     * @param  _idx             索引
     * @return uint8_t          对应颜色
     */
    uint8_t              operator[](uint8_t _idx) const;

    /**
     * @brief 获取每像素大小
     * @return size_t           像素大小，单位为字节
     */
    static size_t        bpp(void);

    /**
     * @brief 转换为 uint32_t
     * @return uint32_t         结果
     */
    uint32_t             to_uint32(void) const;

    friend std::ostream& operator<<(std::ostream& _os, const color_t& _color) {
        _os << std::hex;
        _os << "RGBA[ 0x" << +_color.channel_r << ", 0x" << +_color.channel_g
            << ", 0x" << +_color.channel_b << ", 0x" << +_color.channel_a
            << " ]";
        _os << std::dec;

        return _os;
    }
};

#endif /* SIMPLERENDER_COLOR_H */