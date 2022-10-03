
/**
 * @file draw3d_test.cpp
 * @brief draw3d_t 测试
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2022-09-06
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleRenderer
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2022-09-06<td>Zone.N<td>创建文件
 * </table>
 */

#include "draw3d.h"
#include "display.h"
#include "vector.hpp"
#include "gtest/gtest.h"

static constexpr const uint32_t                  WIDTH  = 1920;
static constexpr const uint32_t                  HEIGHT = 1080;
[[maybe_unused]] static constexpr const uint32_t RED    = 0xFFFF0000;
[[maybe_unused]] static constexpr const uint32_t GREEN  = 0xFF00FF00;
[[maybe_unused]] static constexpr const uint32_t BLUE   = 0xFF0000FF;
[[maybe_unused]] static constexpr const uint32_t WHITE  = 0xFFFFFFFF;
[[maybe_unused]] static constexpr const uint32_t BLACK  = 0xFF000000;

TEST(draw2d_t, test1) {
    auto     framebuffer = std::make_shared<framebuffer_t>(WIDTH, HEIGHT);

    draw3d_t draw3d(framebuffer);
    draw3d.line(0, HEIGHT - 1, WIDTH - 1, 0, WHITE);
    draw3d.line(WIDTH - 1, HEIGHT - 1, 0, 0, WHITE);
    draw3d.line(WIDTH - 1, HEIGHT / 2, 0, HEIGHT / 2, WHITE);
    draw3d.line(WIDTH / 2, 0, WIDTH / 2, HEIGHT - 1, WHITE);

    vector4f_t v0(80, 80);
    vector4f_t v1(800, 800);
    vector4f_t v2(50, 900);
    draw3d.line(v2.x, v2.y, v0.x, v0.y, GREEN);
    draw3d.line(v0.x, v0.y, v2.x, v2.y, GREEN);

    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 + 100, HEIGHT / 2 + 60, GREEN);
    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 + 50, HEIGHT / 2 + 100, GREEN);

    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 - 80, HEIGHT / 2 - 100, GREEN);
    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 - 100, HEIGHT / 2 - 50, GREEN);

    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 - 50, HEIGHT / 2 + 100, GREEN);
    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 - 100, HEIGHT / 2 + 90, GREEN);

    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 + 90, HEIGHT / 2 - 100, GREEN);
    draw3d.line(WIDTH / 2, HEIGHT / 2, WIDTH / 2 + 100, HEIGHT / 2 - 50, GREEN);

    draw3d.line(10, 20, 100, 200, RED);

    vector4f_t v3(830, 984);
    vector4f_t v4(400, 874);
    vector4f_t v5(505, 456);
    draw3d.triangle2d(v5, v3, v4, GREEN);
    draw3d.triangle2d(v0, v1, v2, RED);

    display_t display(framebuffer);
    display.loop();
    return;
}
