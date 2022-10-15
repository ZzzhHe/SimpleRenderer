
/**
 * @file display.cpp
 * @brief 显示
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-09-05
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-05<td>Zone.N<td>创建文件
 * </table>
 */

#include "iostream"

#include "camera.h"
#include "display.h"
#include "log.hpp"

uint8_t display_t::ARGB2A(const uint32_t _color) {
    return (_color >> 24) & 0xFF;
}

uint8_t display_t::ARGB2R(const uint32_t _color) {
    return (_color >> 16) & 0xFF;
}

uint8_t display_t::ARGB2G(const uint32_t _color) {
    return (_color >> 8) & 0xFF;
}

uint8_t display_t::ARGB2B(const uint32_t _color) {
    return (_color >> 0) & 0xFF;
}

void display_t::pixel(SDL_Surface* _surface, const uint32_t _x,
                      const uint32_t _y, const uint8_t _a, const uint8_t _r,
                      const uint8_t _g, const uint8_t _b) {
    // 加锁
    try {
        auto ret = SDL_LockSurface(_surface);
        if (ret != 0) {
            throw std::runtime_error(log(SDL_GetError()));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    // 判断颜色深度
    int bpp = _surface->format->BytesPerPixel;
    // 只支持深度为 4 的，即 32 位色
    if (bpp != 4) {
        auto err = log("bpp != 4 ") + "Only support "
                 + SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(sdl_window));
        throw std::runtime_error(err);
    }
    // 计算像素位置
    auto p = (uint8_t*)_surface->pixels + _y * _surface->pitch + _x * bpp;
    // 写入
    *(uint32_t*)p = SDL_MapRGBA(_surface->format, _r, _g, _b, _a);
    SDL_UnlockSurface(_surface);
    return;
}

void display_t::fill(void) {
    auto surface = SDL_GetWindowSurface(sdl_window);
    try {
        if (surface == nullptr) {
            throw std::runtime_error(log(SDL_GetError()));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    uint32_t color = 0xFFFFFFFF;
    // 填充整个屏幕
    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            color = framebuffer->get_color_buffer()(i, j);
            pixel(surface, i, j, ARGB2A(color), ARGB2R(color), ARGB2G(color),
                  ARGB2B(color));
        }
    }
    return;
}

void display_t::apply_surface(uint32_t _x, uint32_t _y,
                              SDL_Surface* _src) const {
    SDL_Rect offset;
    offset.x = _x;
    offset.y = _y;
    SDL_BlitSurface(_src, NULL, SDL_GetWindowSurface(sdl_window), &offset);
    return;
}

void display_t::show_fps(void) {
    fps_text    = "FPS: " + std::to_string(fps);
    fps_surface = TTF_RenderText_Solid(font, fps_text.c_str(), fps_color);
    if (fps_surface == nullptr) {
        throw std::runtime_error(log(TTF_GetError()));
    }
    apply_surface(4, 0, fps_surface);
    return;
}

display_t::display_t(std::shared_ptr<framebuffer_t> _framebuffer)
    : framebuffer(_framebuffer) {
    width  = framebuffer->get_width();
    height = framebuffer->get_height();
    // 初始化 sdl
    try {
        auto ret = SDL_Init(SDL_INIT_VIDEO);
        if (ret != 0) {
            throw std::runtime_error(log(SDL_GetError()));
        }
        // 创建窗口，居中，可见
        sdl_window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, width, height,
                                      SDL_WINDOW_SHOWN);
        if (sdl_window == nullptr) {
            SDL_Quit();
            throw std::runtime_error(log(SDL_GetError()));
        }

        // 文字显示
        if (TTF_Init() != 0) {
            SDL_DestroyWindow(sdl_window);
            SDL_Quit();
            throw std::runtime_error(log(TTF_GetError()));
        }
        // 打开字体库
        font = TTF_OpenFont(font_file_path.c_str(), fone_size);
        if (font == nullptr) {
            TTF_Quit();
            SDL_DestroyWindow(sdl_window);
            SDL_Quit();
            throw std::runtime_error(log(TTF_GetError()));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    return;
}

display_t::~display_t(void) {
    // 回收资源
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void display_t::input_handler(void) {
    // 捕获事件
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            // 键盘事件
            case SDL_KEYDOWN: {
                switch (sdl_event.key.keysym.sym) {
                    case SDLK_ESCAPE: {
                        // 如果是 esc 键则将 is_should_quit 置位
                        is_should_quit = true;
                        break;
                    }
                    case SDLK_a: {
                        camera.get_pos().x--;
                        break;
                    }
                    case SDLK_d: {
                        camera.get_pos().x++;
                        break;
                    }
                    case SDLK_SPACE: {
                        camera.get_pos().y++;
                        break;
                    }
                    case SDLK_LCTRL: {
                        camera.get_pos().y--;
                        break;
                    }
                    case SDLK_w: {
                        camera.get_pos().z++;
                        break;
                    }
                    case SDLK_s: {
                        camera.get_pos().z--;
                        break;
                    }
                    default: {
                        // 输出按键名
                        printf("key %s down！\n",
                               SDL_GetKeyName(sdl_event.key.keysym.sym));
                    }
                }

                break;
            }
            // 鼠标移动
            case SDL_MOUSEMOTION: {
                camera.get_target().x += sdl_event.motion.xrel;
                camera.get_target().y += sdl_event.motion.yrel;
                break;
            }
            // 鼠标点击
            case SDL_MOUSEBUTTONDOWN: {
                break;
            }
            // 鼠标滚轮
            case SDL_MOUSEWHEEL: {
                break;
            }
            case SDL_QUIT: {
                is_should_quit = true;
                break;
            }
        }
    }
    return;
}

void display_t::loop(void) {
    uint64_t sec = 0;
    // 主循环
    while (is_should_quit == false) {
        auto start = us();
        // 处理输入
        input_handler();
        // 填充窗口
        framebuffer->clear();
        fill();
        // 显示 fps
        show_fps();
        // 刷新窗口
        try {
            auto ret = SDL_UpdateWindowSurface(sdl_window);
            if (ret != 0) {
                throw std::runtime_error(log(SDL_GetError()));
            }
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return;
        }
        fps++;
        auto end = us();
        sec      += end - start;
        if (sec >= 1000000) {
            // std::cout << "fps_window: " << fps << std::endl;
            fps = 0;
            sec = 0;
        }
    }
    return;
}
