#pragma once

#include <imgui.h>

#include <scene.h>

namespace update {
void setup_globals();
void refresh_ubos();
void refresh_view(const glm::mat4 &view);
void per_frame_update(blackhole::scene &s);
} // namespace update
