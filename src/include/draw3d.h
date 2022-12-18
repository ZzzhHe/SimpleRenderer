
/**
 * @file draw3d.h
 * @brief 三维绘制
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-09-14
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-14<td>Zone.N<td>创建文件
 * </table>
 */

#ifndef SIMPLERENDER_DRAW3D_H
#define SIMPLERENDER_DRAW3D_H

#include "memory"

#include "color.h"
#include "config.h"
#include "framebuffer.h"
#include "light.h"
#include "matrix.hpp"
#include "model.h"
#include "shader.h"
#include "vector.hpp"

/**
 * @brief 绘制封装
 * @todo 双缓冲区
 */
class draw3d_t {
private:
    /// @brief 配置信息
    std::shared_ptr<config_t>      config;
    /// @brief 帧缓冲
    std::shared_ptr<framebuffer_t> framebuffer;
    /// @brief 要使用的着色器
    const shader_base_t&           shader;
    /// @brief 窗口宽度
    uint32_t                       width;
    /// @brief 窗口高度
    uint32_t                       height;

public:
    /// @brief 光照方向，左手系，x 向右，y 向下，z 正方向为屏幕由内向外
    light_t light;

    /**
     * @brief 计算重心坐标
     * @param  _p0              三角形的第一个点
     * @param  _p1              三角形的第二个点
     * @param  _p2              三角形的第三个点
     * @param  _p               要判断的点
     * @return std::pair<bool, vector4f_t>
     *  第一个返回为 _p 是否在三角形内，第二个为重心坐标
     * @see http://blackpawn.com/texts/pointinpoly/
     * solve
     *     P = A + s * AB + t * AC  -->  AP = s * AB + t * AC
     * then
     *     s = (AC.y * AP.x - AC.x * AP.y) / (AB.x * AC.y - AB.y * AC.x)
     *     t = (AB.x * AP.y - AB.y * AP.x) / (AB.x * AC.y - AB.y * AC.x)
     *
     * notice
     *     P = A + s * AB + t * AC
     *       = A + s * (B - A) + t * (C - A)
     *       = (1 - s - t) * A + s * B + t * C
     * then
     *     weight_A = 1 - s - t
     *     weight_B = s
     *     weight_C = t
     */
    static std::pair<bool, vector4f_t>
    get_barycentric_coord(const vector4f_t& _p0, const vector4f_t& _p1,
                          const vector4f_t& _p2, const vector4f_t& _p);

    /**
     * @brief 深度插值，由重心坐标计算出对应点的深度值
     * @param  _v0              点 1
     * @param  _v1              点 2
     * @param  _v2              点 3
     * @param  _barycentric_coord   重心坐标
     * @return framebuffer_t::depth_t 深度值
     */
    static framebuffer_t::depth_t
    interpolate_depth(const framebuffer_t::depth_t& _v0,
                      const framebuffer_t::depth_t& _v1,
                      const framebuffer_t::depth_t& _v2,
                      const vector4f_t&             _barycentric_coord);

    /**
     * @brief 填充三角形，传入的顶点包含更多信息
     * @param  _v0              第一个顶点
     * @param  _v1              第二个顶点
     * @param  _v2              第三个顶点
     * @param  _normal          面的法向量
     * @todo 多线程支持
     */
    void
         triangle(const model_t::vertex_t& _v0, const model_t::vertex_t& _v1,
                  const model_t::vertex_t& _v2, const model_t::normal_t& _normal);

    /**
     * @brief 填充三角形，传入面信息
     * @param  _face            面
     */
    void triangle(const model_t::face_t& _face);

public:
    /**
     * @brief 构造函数，不使用
     */
    draw3d_t(void) = delete;

    /**
     * @brief 构造函数
     * @param  _config          配置信息
     * @param  _framebuffer     帧缓冲
     * @param  _shader          着色器
     */
    draw3d_t(const std::shared_ptr<config_t>&      _config,
             const std::shared_ptr<framebuffer_t>& _framebuffer,
             const shader_base_t&                  _shader);

    /**
     * @brief 析构函数
     */
    ~draw3d_t(void);

    /**
     * @brief 画直线 Bresenham 算法
     * @param  _x0              第一个点的 x 坐标
     * @param  _y0              第一个点的 y 坐标
     * @param  _x1              第二个点的 x 坐标
     * @param  _y1              第二个点的 y 坐标
     * @param  _color           直线颜色
     * @todo 多线程支持
     */
    void line(int32_t _x0, int32_t _y0, int32_t _x1, int32_t _y1,
              const color_t& _color);

    /**
     * @brief 填充三角形
     * @param  _v0              第一个顶点
     * @param  _v1              第二个顶点
     * @param  _v2              第三个顶点
     * @param  _color           填充的颜色
     * @todo 多线程支持
     */
    void triangle(const vector4f_t& _v0, const vector4f_t& _v1,
                  const vector4f_t& _v2, const color_t& _color);

    /**
     * @brief 绘制整个模型，指定变换矩阵
     * @param  _model           模型信息
     */
    void model(const model_t& _model);
};

#endif /* SIMPLERENDER_DRAW3D_H */
