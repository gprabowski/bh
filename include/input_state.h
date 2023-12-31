#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <bitset>
#include <glad/glad.h>

#include <imgui.h>

struct input_state {
  enum mouse_button : int { left = 0, right = 1, middle = 2, other = 3 };

  float yaw{-90.0f};
  float pitch{0.0f};
  float roll{0.0f};

  glm::vec3 cam_pos{0.0f, 0.0f, 0.0f};
  glm::vec3 cam_front{0.0f, 0.0f, -1.0f};
  glm::vec3 cam_up{0.0f, 1.0f, 0.0f};

  glm::vec2 last_mouse{0.0f, 0.0f};

  std::bitset<1024> pressed{0x0};
  std::bitset<1024> just_pressed{0x0};
  std::bitset<4> mouse_pressed{0};
  std::bitset<4> mouse_just_pressed{0};

public:
  static input_state &get_input_state() {
    static input_state as;
    return as;
  }

  input_state &operator=(const input_state &r) = delete;
  input_state &operator=(const input_state &&r) = delete;
  input_state(const input_state &) = delete;
  input_state(const input_state &&) = delete;

private:
  input_state(){};
};
