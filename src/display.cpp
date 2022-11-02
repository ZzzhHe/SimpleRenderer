
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
    /// @todo 性能瓶颈
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
    // 填充整个屏幕
    /// @todo 优化，看看能不能直接写整个 buf
    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            /// @todo 性能瓶颈
            auto color = color_t(framebuffer.get_color_buffer()(i, j));
            // auto color = framebuffer.get_color_buffer()(i, j).to_argb();
            /// @todo 性能瓶颈
            pixel(surface, i, j, color[0], color[1], color[2], color[3]);
        }
    }
    // auto color_argb = framebuffer.get_color_buffer();
    // auto res        = SDL_UpdateTexture(sdl_texture, nullptr, color_argb,
    //                                     width * color_t::size());
    // if (res == -1) {
    //     throw std::runtime_error(log(SDL_GetError()));
    // }
    //
    // res = SDL_RenderCopy(sdl_renderer, sdl_texture, nullptr, nullptr);
    // if (res == -1) {
    //     throw std::runtime_error(log(SDL_GetError()));
    // }

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

void display_t::show_fps(const uint32_t _fps) {
    auto fps_text = FPS + std::to_string(_fps);
    fps_surface   = TTF_RenderText_Solid(font, fps_text.c_str(), fps_color);
    if (fps_surface == nullptr) {
        throw std::runtime_error(log(TTF_GetError()));
    }
    apply_surface(FPS_POS_X, FPS_POS_Y, fps_surface);
    return;
}

display_t::display_t(framebuffer_t& _framebuffer, camera_t& _camera,
                     event_callback_t& _event_callback)
    : framebuffer(_framebuffer),
      camera(_camera),
      event_callback(_event_callback) {
    width  = framebuffer.get_width();
    height = framebuffer.get_height();
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

        // sdl_renderer = SDL_CreateRenderer(sdl_window, -1,
        //                                   SDL_RENDERER_ACCELERATED
        //                                     | SDL_RENDERER_PRESENTVSYNC);
        // if (sdl_renderer == nullptr) {
        //     SDL_DestroyWindow(sdl_window);
        //     SDL_Quit();
        //     throw std::runtime_error(log(SDL_GetError()));
        // }
        //
        // sdl_texture
        //   = SDL_CreateTexture(sdl_renderer,
        //                       SDL_GetWindowPixelFormat(sdl_window),
        //                       SDL_TEXTUREACCESS_STREAMING, width, height);
        // if (sdl_renderer == nullptr) {
        //     SDL_DestroyRenderer(sdl_renderer);
        //     SDL_DestroyWindow(sdl_window);
        //     SDL_Quit();
        //     throw std::runtime_error(log(SDL_GetError()));
        // }

        // 文字显示
        if (TTF_Init() != 0) {
            // SDL_DestroyTexture(sdl_texture);
            // SDL_DestroyRenderer(sdl_renderer);
            SDL_DestroyWindow(sdl_window);
            SDL_Quit();
            throw std::runtime_error(log(TTF_GetError()));
        }
        // 打开字体库
        font = TTF_OpenFont(font_file_path, font_size);
        if (font == nullptr) {
            TTF_Quit();
            // SDL_DestroyTexture(sdl_texture);
            // SDL_DestroyRenderer(sdl_renderer);
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
    // SDL_DestroyTexture(sdl_texture);
    // SDL_DestroyRenderer(sdl_renderer);
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
                        event_callback.key_a();
                        break;
                    }
                    case SDLK_d: {
                        event_callback.key_d();
                        break;
                    }
                    case SDLK_SPACE: {
                        event_callback.key_space();
                        break;
                    }
                    case SDLK_z: {
                        event_callback.key_z();
                        break;
                    }
                    case SDLK_LCTRL: {
                        event_callback.key_left_ctrl();
                        break;
                    }
                    case SDLK_w: {
                        event_callback.key_w();
                        break;
                    }
                    case SDLK_s: {
                        event_callback.key_s();
                        break;
                    }
                    case SDLK_LSHIFT: {
                        event_callback.key_left_shift();
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
                event_callback.key_mouse_motion();
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
    uint64_t sec    = 0;
    uint32_t frames = 0;
    uint32_t fps    = 0;
    auto     start  = us();
    auto     end    = us();
    // 主循环
    while (is_should_quit == false) {
        start = us();
        // 处理输入
        input_handler();
        // 填充窗口
        framebuffer.clear();
        fill();
        // 显示 fps
        show_fps(fps);
        // 刷新窗口
        // SDL_RenderPresent(sdl_renderer);
        try {
            auto ret = SDL_UpdateWindowSurface(sdl_window);
            if (ret != 0) {
                throw std::runtime_error(log(SDL_GetError()));
            }
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return;
        }
        frames++;
        end = us();
        sec += end - start;
        if (sec >= US2S) {
            // std::cout << "fps_window: " << fps << std::endl;
            fps    = frames;
            frames = 0;
            sec    = 0;
        }
    }
    return;
}
