#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <scene.h>

namespace render {
void begin_frame(uint64_t &b, blackhole::scene &s);
void end_frame(GLFWwindow *w, uint64_t &begin_time);
void render_window_gui(blackhole::scene &s);
void render_viewport(blackhole::scene &s);
} // namespace render
