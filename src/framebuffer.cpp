
/**
 * @file framebuffer.cpp
 * @brief 缓冲区抽象
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-09-03
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-03<td>Zone.N<td>创建文件
 * </table>
 */

#include "cassert"
#include "cmath"
#include "cstring"
#include "iostream"

#include "framebuffer.h"
#include "log.hpp"

framebuffer_t::color_buffer_t::color_buffer_t(
  const color_buffer_t& _color_buffer)
    : width(_color_buffer.width), height(_color_buffer.height) {
    try {
        color_arr = new color_t[width * height];
    } catch (const std::bad_alloc& e) {
        std::cerr << log(e.what()) << std::endl;
    }
    // 复制数据
    std::copy(_color_buffer.color_arr,
              _color_buffer.color_arr + _color_buffer.length(), color_arr);
    return;
}

framebuffer_t::color_buffer_t::color_buffer_t(const uint32_t _w,
                                              const uint32_t _h,
                                              const color_t& _color)
    : width(_w), height(_h) {
    try {
        color_arr = new color_t[width * height];
    } catch (const std::bad_alloc& e) {
        std::cerr << log(e.what()) << std::endl;
    }
    std::fill_n(color_arr, width * height, _color);
    return;
}

framebuffer_t::color_buffer_t::~color_buffer_t(void) {
    width  = 0;
    height = 0;
    if (color_arr != nullptr) {
        delete[] color_arr;
        color_arr = nullptr;
    }
    return;
}

framebuffer_t::color_buffer_t&
framebuffer_t::color_buffer_t::operator=(const color_buffer_t& _color_buffer) {
    if (width != _color_buffer.width || height != _color_buffer.height) {
        width  = _color_buffer.width;
        height = _color_buffer.height;
        if (color_arr != nullptr) {
            delete[] color_arr;
            color_arr = nullptr;
        }
        try {
            color_arr = new color_t[width * height];
        } catch (const std::bad_alloc& e) {
            std::cerr << log(e.what()) << std::endl;
        }
    }
    std::copy(_color_buffer.color_arr,
              _color_buffer.color_arr + width * height * sizeof(color_t),
              color_arr);

    return *this;
}

color_t& framebuffer_t::color_buffer_t::operator()(const uint32_t _x,
                                                   const uint32_t _y) {
    if (_x >= width) {
        throw std::invalid_argument(log("_x >= width"));
    }
    if (_y >= height) {
        throw std::invalid_argument(log("_y >= height"));
    }
    return color_arr[_y * width + _x];
}

const color_t
framebuffer_t::color_buffer_t::operator()(const uint32_t _x,
                                          const uint32_t _y) const {
    if (_x >= width) {
        throw std::invalid_argument(log("_x >= width"));
    }
    if (_y >= height) {
        throw std::invalid_argument(log("_y >= height"));
    }
    return color_t(color_arr[_y * width + _x]);
}

void framebuffer_t::color_buffer_t::clear(const color_t& _color) {
    std::fill_n(color_arr, width * height, _color);
    return;
}

size_t framebuffer_t::color_buffer_t::length(void) const {
    return width * height * color_t::bpp();
}

const color_t* framebuffer_t::color_buffer_t::to_arr(void) {
    return color_arr;
}

framebuffer_t::depth_buffer_t::depth_buffer_t(
  const depth_buffer_t& _depth_buffer)
    : width(_depth_buffer.width), height(_depth_buffer.height) {
    try {
        depth_arr = new depth_t[width * height];
    } catch (const std::bad_alloc& e) {
        std::cerr << log(e.what()) << std::endl;
    }
    // 复制数据
    std::copy(_depth_buffer.depth_arr,
              _depth_buffer.depth_arr + _depth_buffer.length(), depth_arr);
    return;
}

framebuffer_t::depth_buffer_t::depth_buffer_t(const uint32_t _w,
                                              const uint32_t _h,
                                              const depth_t& _depth)
    : width(_w), height(_h) {
    try {
        depth_arr = new depth_t[width * height];
    } catch (const std::bad_alloc& e) {
        std::cerr << log(e.what()) << std::endl;
    }
    std::fill_n(depth_arr, width * height, _depth);
    return;
}

framebuffer_t::depth_buffer_t::~depth_buffer_t(void) {
    width  = 0;
    height = 0;
    if (depth_arr != nullptr) {
        delete[] depth_arr;
    }
    return;
}

framebuffer_t::depth_buffer_t&
framebuffer_t::depth_buffer_t::operator=(const depth_buffer_t& _depth_buffer) {
    if (width != _depth_buffer.width || height != _depth_buffer.height) {
        width  = _depth_buffer.width;
        height = _depth_buffer.height;
        if (depth_arr != nullptr) {
            delete[] depth_arr;
        }
        try {
            depth_arr = new depth_t[width * height];
        } catch (const std::bad_alloc& e) {
            std::cerr << log(e.what()) << std::endl;
        }
    }
    std::copy(_depth_buffer.depth_arr,
              _depth_buffer.depth_arr + width * height * sizeof(depth_t),
              depth_arr);

    return *this;
}

framebuffer_t::depth_t&
framebuffer_t::depth_buffer_t::operator()(const uint32_t _x,
                                          const uint32_t _y) {
    if (_x >= width) {
        throw std::invalid_argument(log("_x >= width"));
    }
    if (_y >= height) {
        throw std::invalid_argument(log("_y >= height"));
    }
    return depth_arr[_y * width + _x];
}

framebuffer_t::depth_t
framebuffer_t::depth_buffer_t::operator()(const uint32_t _x,
                                          const uint32_t _y) const {
    if (_x >= width) {
        throw std::invalid_argument(log("_x >= width"));
    }
    if (_y >= height) {
        throw std::invalid_argument(log("_y >= height"));
    }
    return depth_arr[_y * width + _x];
}

void framebuffer_t::depth_buffer_t::clear(const depth_t& _depth) {
    if (std::isnan(_depth)) {
        throw(log("std::isnan(_depth)"));
    }
    std::fill_n(depth_arr, width * height, _depth);
    return;
}

size_t framebuffer_t::depth_buffer_t::length(void) const {
    return width * height * sizeof(depth_t);
}

framebuffer_t::framebuffer_t(const uint32_t _width, const uint32_t _height)
    : width(_width),
      height(_height),
      color_buffer(color_buffer_t(_width, _height)),
      depth_buffer(depth_buffer_t(_width, _height)) {
    std::atomic_init(&is_should_draw, true);
    return;
}

framebuffer_t::framebuffer_t(const framebuffer_t& _framebuffer)
    : color_buffer(_framebuffer.color_buffer),
      depth_buffer(_framebuffer.depth_buffer) {
    if (width != _framebuffer.get_width()) {
        throw std::invalid_argument(log("width != _framebuffer.get_width()"));
    }
    if (height != _framebuffer.get_height()) {
        throw std::invalid_argument(log("height != _framebuffer.get_height()"));
    }
    return;
}

framebuffer_t::~framebuffer_t(void) {
    return;
}

framebuffer_t& framebuffer_t::operator=(const framebuffer_t& _framebuffer) {
    if (this == &_framebuffer) {
        throw std::runtime_error(log("this == &_framebuffer"));
    }
    if (width != _framebuffer.get_width()) {
        throw std::invalid_argument(log("width != _framebuffer.get_width()"));
    }
    if (height != _framebuffer.get_height()) {
        throw std::invalid_argument(log("height != _framebuffer.get_height()"));
    }
    color_buffer = _framebuffer.color_buffer;
    depth_buffer = _framebuffer.depth_buffer;
    return *this;
}

uint32_t framebuffer_t::get_width(void) const {
    return width;
}

uint32_t framebuffer_t::get_height(void) const {
    return height;
}

void framebuffer_t::clear(const color_t& _color, const depth_t& _depth) {
    if (std::isnan(_depth)) {
        throw(log("std::isnan(_depth)"));
    }
    color_buffer.clear(_color);
    depth_buffer.clear(_depth);
    return;
}

void framebuffer_t::pixel(const uint32_t _x, const uint32_t _y,
                          const color_t& _color, const depth_t& _depth) {
    if (_x >= width) {
        throw std::invalid_argument(log("_x >= width"));
    }
    if (_y >= height) {
        throw std::invalid_argument(log("_y >= height"));
    }
    if (std::isnan(_depth)) {
        throw(log("std::isnan(_depth)"));
    }
    /// @todo 性能瓶颈
    // std::lock_guard<std::mutex> color_buffer_lock(color_buffer_mutex);
    // std::lock_guard<std::mutex> depth_buffer_lock(depth_buffer_mutex);
    color_buffer(_x, _y) = _color;
    depth_buffer(_x, _y) = _depth;
    return;
}

framebuffer_t::color_buffer_t& framebuffer_t::get_color_buffer(void) {
    return color_buffer;
}

const framebuffer_t::color_buffer_t&
framebuffer_t::get_color_buffer(void) const {
    return color_buffer;
}

framebuffer_t::depth_buffer_t& framebuffer_t::get_depth_buffer(void) {
    return depth_buffer;
}

framebuffer_t::depth_t&
framebuffer_t::get_depth_buffer(const uint32_t _x, const uint32_t _y) {
    return depth_buffer(_x, _y);
}

const framebuffer_t::depth_buffer_t&
framebuffer_t::get_depth_buffer(void) const {
    return depth_buffer;
}

framebuffer_t::depth_t
framebuffer_t::get_depth_buffer(const uint32_t _x, const uint32_t _y) const {
    return depth_buffer(_x, _y);
}
