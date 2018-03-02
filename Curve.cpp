#include <iostream>
#include <cmath>
#include "Curve.h"
#include "Window.h"

//! Static member variables
std::vector< GLfloat >  Curve::m_vertices;
size_t                  Curve::m_cnt = 0;

//! Curve constructor
Curve::Curve() {
  glGenVertexArrays( 1, &m_VAO );
  glGenBuffers( 1, &m_VBO );

  glBindVertexArray( m_VAO );

  glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
  glBufferData( GL_ARRAY_BUFFER, Curve::m_vertices.size() * sizeof( GLfloat ),
                &Curve::m_vertices[0], GL_STATIC_DRAW );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ),
                         (GLvoid *) 0 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
}

/** Static Bezier curve (cubic, 4 control points) generation function
 *  C0 & C1 continuous
 **/
void Curve::genCurve( const glm::vec3 &i_p0, const glm::vec3 &i_p1,
                      const glm::vec3 &i_p2, const glm::vec3 &i_p3 ) {
  float l_val;

  for( float l_t = 0.0f; l_t <= 0.995f; l_t += (1.0f / 200.0f) ) {
    l_val = glm::dot( glm::vec4( i_p0.x, i_p1.x, i_p2.x, i_p3.x ),
                      glm::mat4( glm::vec4( -1.0f,  3.0f, -3.0f, 1.0f ),
                                 glm::vec4(  3.0f, -6.0f,  3.0f, 0.0f ),
                                 glm::vec4( -3.0f,  3.0f,  0.0f, 0.0f ),
                                 glm::vec4(  1.0f,  0.0f,  0.0f, 0.0f ) ) *
                                 glm::vec4( powf( l_t, 3.0f ), powf( l_t, 2.0f ),
                                                  l_t, 1.0f ) );
    Curve::m_vertices.push_back( l_val );

    l_val = glm::dot( glm::vec4( i_p0.y, i_p1.y, i_p2.y, i_p3.y ),
                      glm::mat4( glm::vec4( -1.0f,  3.0f, -3.0f, 1.0f ),
                                 glm::vec4(  3.0f, -6.0f,  3.0f, 0.0f ),
                                 glm::vec4( -3.0f,  3.0f,  0.0f, 0.0f ),
                                 glm::vec4(  1.0f,  0.0f,  0.0f, 0.0f ) ) *
                                 glm::vec4( powf( l_t, 3.0f ), powf( l_t, 2.0f ),
                                                  l_t, 1.0f ) );
    Curve::m_vertices.push_back( l_val );

    l_val = glm::dot( glm::vec4( i_p0.z, i_p1.z, i_p2.z, i_p3.z ),
                      glm::mat4( glm::vec4( -1.0f,  3.0f, -3.0f, 1.0f ),
                                 glm::vec4(  3.0f, -6.0f,  3.0f, 0.0f ),
                                 glm::vec4( -3.0f,  3.0f,  0.0f, 0.0f ),
                                 glm::vec4(  1.0f,  0.0f,  0.0f, 0.0f ) ) *
                                 glm::vec4( powf( l_t, 3.0f ), powf( l_t, 2.0f ),
                                                  l_t, 1.0f ) );
    Curve::m_vertices.push_back( l_val );
  }
}

//! Function to draw the line segments for the Bezier curve
void Curve::draw( const GLuint &i_shaderProgram ) {
  glm::mat4 l_modelView = Window::m_V;

  m_uProjection = glGetUniformLocation( i_shaderProgram, "projection" );
  m_uModelView  = glGetUniformLocation( i_shaderProgram, "modelView"  );

  glUniformMatrix4fv( m_uProjection, 1, GL_FALSE, &Window::m_P[0][0] );
  glUniformMatrix4fv( m_uModelView,  1, GL_FALSE, &l_modelView[0][0] );

  glBindVertexArray( m_VAO );
  glLineWidth( 2.0f );

  glDrawArrays( GL_LINE_LOOP, 0, Curve::m_vertices.size() / 3.0f );

  glBindVertexArray( 0 );
}
