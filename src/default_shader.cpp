
/**
 * @file default_shader.cpp
 * @brief 默认着色器实现
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-11-01
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-11-01<td>Zone.N<td>创建文件
 * </table>
 */

#include "default_shader.h"

const std::pair<bool, const vector4f_t>
default_shader_t::get_barycentric_coord(const vector4f_t& _p0,
                                        const vector4f_t& _p1,
                                        const vector4f_t& _p2,
                                        const vector4f_t& _p) {
    auto ab   = _p1 - _p0;
    auto ac   = _p2 - _p0;
    auto ap   = _p - _p0;

    auto deno = (ab.x * ac.y - ab.y * ac.x);
    if (deno == 0) {
        return std::pair<bool, const vector4f_t>(false, vector4f_t());
    }

    auto s       = (ac.y * ap.x - ac.x * ap.y) / deno;
    auto t       = (ab.x * ap.y - ab.y * ap.x) / deno;
    auto weights = vector4f_t(1 - s - t, s, t);

    auto res     = ((weights.x <= 1) && (weights.x >= 0))
            && ((weights.y <= 1) && (weights.y >= 0))
            && ((weights.z <= 1) && (weights.z >= 0));

    return std::pair<bool, const vector4f_t>(res, weights);
}

framebuffer_t::depth_t
default_shader_t::interpolate_depth(const framebuffer_t::depth_t& _depth0,
                                    const framebuffer_t::depth_t& _depth1,
                                    const framebuffer_t::depth_t& _depth2,
                                    const vector4f_t& _barycentric_coord) {
    auto z = _depth0 * _barycentric_coord.x;
    z      += _depth1 * _barycentric_coord.y;
    z      += _depth2 * _barycentric_coord.z;
    return z;
}

framebuffer_t::color_t
default_shader_t::interpolate_color(const model_t::color_t&  _c0,
                                    const model_t::color_t&  _c1,
                                    const model_t::color_t&  _c2,
                                    const vector4f_t&        _barycentric_coord,
                                    const model_t::normal_t& _normal) {
    // 光照强度
    // auto intensity = _normal * light;
    auto intensity = 0;

    auto color_v   = model_t::color_t(
      _c0.x * _barycentric_coord.x + _c1.x * _barycentric_coord.y
        + _c2.x * _barycentric_coord.z,
      _c0.y * _barycentric_coord.x + _c1.y * _barycentric_coord.y
        + _c2.y * _barycentric_coord.z,
      _c0.z * _barycentric_coord.x + _c1.z * _barycentric_coord.y
        + _c2.z * _barycentric_coord.z);
    auto color = framebuffer_t::ARGB(
      std::numeric_limits<uint8_t>::max(),
      static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * color_v.x
                           * intensity),
      static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * color_v.y
                           * intensity),
      static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * color_v.z
                           * intensity));
    return color;
}

default_shader_t::default_shader_t(void) {
    return;
}

default_shader_t::~default_shader_t(void) {
    return;
}

const shader_vertex_out_t
default_shader_t::vertex(const shader_vertex_in_t& _shader_vertex_in) {
    /// @todo 处理变换
    auto face(_shader_vertex_in.face);
    // 变换坐标
    auto mvp = shader_data.project_matrix * shader_data.view_matrix
             * shader_data.model_matrix;
    face.v0.coord = mvp * face.v0.coord;
    face.v1.coord = mvp * face.v1.coord;
    face.v2.coord = mvp * face.v2.coord;
    /// @todo 通过矩阵变换法线
    auto v2v0     = face.v2.coord - face.v0.coord;
    auto v1v0     = face.v1.coord - face.v0.coord;
    auto normal   = (v2v0 ^ v1v0).normalize();
    face.normal   = normal;
    /// @todo 变换贴图
    return shader_vertex_out_t(face);
}

const shader_fragment_out_t
default_shader_t::fragment(const shader_fragment_in_t& _shader_fragment_in) {
    (void)_shader_fragment_in;
    return shader_fragment_out_t();
}
