#ifndef CURVE_H
#define CURVE_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
//! Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Curve {
private:
  GLuint  m_VAO, m_VBO;
  GLuint  m_uProjection, m_uModelView;

public:
  static std::vector< GLfloat > m_vertices;
  static size_t                 m_cnt;

  Curve();

  static void genCurve( const glm::vec3 &i_p0, const glm::vec3 &i_p1,
                        const glm::vec3 &i_p2, const glm::vec3 &i_p3 );

  void draw( const GLuint &i_shaderProgram );
};

#endif
