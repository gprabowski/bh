#pragma once

#include <set>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <imgui.h>

struct frame_state {

  static glm::mat4 view;
  static glm::mat4 proj;

  static glm::vec4 light_pos;
  static glm::vec4 light_color;

  static glm::vec3 bh_pos;
  static float bh_mass;

  static int window_w;
  static int window_h;

  static uint64_t freq;
  static double last_cpu_frame;

  static ImVec2 content_area;
  static ImVec2 content_pos;
};
