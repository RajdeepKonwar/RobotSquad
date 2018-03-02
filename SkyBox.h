#ifndef SKYBOX_H
#define SKYBOX_H

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

class SkyBox {
private:
  glm::mat4 m_toWorld;

  GLuint    m_textureID;
  GLuint    m_VBO, m_VAO;
  GLuint    m_uProjection, m_uModelView;

  unsigned int LoadSkyBox( const std::vector< std::string > &i_faces );

public:
  SkyBox();
  ~SkyBox();

  void draw( GLuint i_shaderProgram );
};

const GLfloat skyBoxVertices[] = {
    // positions
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,
     1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,

    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,

     1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,

    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,

    -1000.0f,  1000.0f, -1000.0f,
     1000.0f,  1000.0f, -1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f, -1000.0f,

    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
     1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
     1000.0f, -1000.0f,  1000.0f
};

#endif
