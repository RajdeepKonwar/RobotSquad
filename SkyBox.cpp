#include <iostream>
#include <fstream>
#include "SkyBox.h"
#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SkyBox::SkyBox() : m_toWorld(glm::mat4( 1.0f )) {
  std::vector< std::string > l_faces { "Skybox_Water222_right.jpg",
                                       "Skybox_Water222_left.jpg",
                                       "Skybox_Water222_top.jpg",
                                       "Skybox_Water222_base.jpg",
                                       "Skybox_Water222_front.jpg",
                                       "Skybox_Water222_back.jpg" };

  glGenVertexArrays( 1, &m_VAO );
  glGenBuffers( 1, &m_VBO );

  glBindVertexArray( m_VAO );

  glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( skyBoxVertices ), &skyBoxVertices,
                GL_STATIC_DRAW );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ),
                         (GLvoid*) 0 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );

  m_textureID = LoadSkyBox( l_faces );
}

SkyBox::~SkyBox() {
  glDeleteVertexArrays( 1, &m_VAO );
  glDeleteBuffers( 1, &m_VBO );
}

unsigned int SkyBox::LoadSkyBox( const std::vector< std::string > &i_faces ) {
  unsigned int l_textureID;
  glGenTextures( 1, &l_textureID );
  glBindTexture( GL_TEXTURE_CUBE_MAP, l_textureID );

  //! Enable single-sided rendering (triangle culling)
  glEnable( GL_CULL_FACE );
  glCullFace( GL_BACK );

  int l_width, l_height, l_nrChannels;
  for( size_t l_i = 0; l_i < i_faces.size(); l_i++ ) {
    unsigned char *l_data = stbi_load( i_faces[l_i].c_str(), &l_width,
                                       &l_height, &l_nrChannels, 0 );

    if( l_data )
      glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + l_i, 0, GL_RGB, l_width,
                    l_height, 0, GL_RGB, GL_UNSIGNED_BYTE, l_data );
    else
      std::cerr << "SkyBox texture failed to load at path: " << i_faces[l_i]
                << std::endl;

    stbi_image_free( l_data );
  }

  //! Use bilinear interpolation
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  //! Use clamp to edge to hide skybox edges
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

  return l_textureID;
}

void SkyBox::draw( GLuint i_shaderProgram ) {
  glm::mat4 l_modelView = Window::m_V * this->m_toWorld;

  m_uProjection = glGetUniformLocation( i_shaderProgram, "projection" );
  m_uModelView  = glGetUniformLocation( i_shaderProgram, "modelView" );

  glUniformMatrix4fv( m_uProjection, 1, GL_FALSE, &Window::m_P[0][0] );
  glUniformMatrix4fv( m_uModelView,  1, GL_FALSE, &l_modelView[0][0] );

  glBindVertexArray( m_VAO );

  glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureID );
  glDrawArrays( GL_TRIANGLES, 0, 36 );

  glBindVertexArray( 0 );
}
