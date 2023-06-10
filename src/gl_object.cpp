#include <gl_object.h>
#include <mesh.h>
#include <transformation.h>

void gl_object::reset_api_elements(blackhole::mesh &m) {
  // allocation or reallocation
  glNamedBufferData(vbo, sizeof(blackhole::vertex_t) * m.vertices.size(),
                    m.vertices.data(), GL_STATIC_DRAW);

  // allocation or reallocation
  glNamedBufferData(ebo, sizeof(m.elements[0]) * m.elements.size(),
                    m.elements.data(), GL_STATIC_DRAW);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(blackhole::vertex_t));
  glVertexArrayElementBuffer(vao, ebo);

  glEnableVertexArrayAttrib(vao, 0);
  glEnableVertexArrayAttrib(vao, 1);
  glEnableVertexArrayAttrib(vao, 2);

  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,
                            offsetof(blackhole::vertex_t, pos));
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,
                            offsetof(blackhole::vertex_t, norm));
  glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE,
                            offsetof(blackhole::vertex_t, tex));

  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribBinding(vao, 1, 0);
  glVertexArrayAttribBinding(vao, 2, 0);
}
