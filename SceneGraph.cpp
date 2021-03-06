#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "SceneGraph.h"
#include "Window.h"

Node::~Node() {}


Transform::Transform( const glm::mat4 &i_mtx ) {
  m_tMtx  = i_mtx;
}

void Transform::addChild( Node *i_child ) {
  m_ptrs.push_back( i_child );
}

//! untested: don't use
void Transform::removeChild() {
  m_ptrs.pop_back();
}

void Transform::draw( const GLuint &i_shaderProgram, const glm::mat4 &i_mtx ) {
  for( std::list< Node * >::iterator l_it = m_ptrs.begin(); l_it != m_ptrs.end(); ++l_it )
    (*l_it)->draw( i_shaderProgram, i_mtx * m_tMtx );
}

void Transform::update( const glm::mat4 &i_mtx ) {
  m_tMtx  = i_mtx;
}


void Geometry::load( const char *i_fileName ) {
  float                 l_val, l_n1, l_n2, l_n3, l_mag, l_lmax;
  float                 l_xcntr, l_ycntr, l_zcntr;
  std::string           l_line, l_next;
  size_t                l_pos;
  unsigned int          l_i, l_index;
  std::vector< float >  l_x, l_y, l_z;

  std::ifstream l_in( i_fileName );
  if( !l_in.is_open() ) {
    std::cerr << "Error loading file " << i_fileName << std::endl;
    exit( EXIT_FAILURE );
  }

  while( getline( l_in, l_line ) ) {
    //! normals
    if( l_line[0] == 'v' && l_line[1] == 'n' ) {
      std::istringstream l_ss( l_line );
      std::vector< std::string > l_tokens;

      while( l_ss ) {
        if( !getline( l_ss, l_next, ' ' ) || l_tokens.size() == 4 )
          break;

        l_tokens.push_back( l_next );
      }

      l_n1  = atof( l_tokens[1].c_str() );
      l_n2  = atof( l_tokens[2].c_str() );
      l_n3  = atof( l_tokens[3].c_str() );

      l_mag = sqrtf( pow( l_n1, 2.0f ) + pow( l_n2, 2.0f ) + pow( l_n3, 2.0f ) );
      l_n1  = (l_n1 / l_mag) * 0.5f + 0.5f;
      l_n2  = (l_n2 / l_mag) * 0.5f + 0.5f;
      l_n3  = (l_n3 / l_mag) * 0.5f + 0.5f;

      //! Populate normals
      this->m_normals.push_back( l_n1 );
      this->m_normals.push_back( l_n2 );
      this->m_normals.push_back( l_n3 );
    }

    //! vertices
    else if ( l_line[0] == 'v' && l_line[1] == ' ' ) {
      std::istringstream l_ss( l_line );
      std::vector< std::string > l_tokens;

      while( l_ss ) {
        if( !getline( l_ss, l_next, ' ' ) || l_tokens.size() == 4 )
          break;

        l_tokens.push_back( l_next );
      }

      //! Populate vertices
      l_val = atof( l_tokens[1].c_str() );
      this->m_vertices.push_back( l_val );
      l_x.push_back( l_val );

      l_val = atof( l_tokens[2].c_str() );
      this->m_vertices.push_back( l_val );
      l_y.push_back( l_val );

      l_val = atof( l_tokens[3].c_str() );
      this->m_vertices.push_back( l_val );
      l_z.push_back( l_val );
    }

    //! faces
    else if( l_line[0] == 'f' ) {
      std::istringstream l_ss( l_line );
      std::vector< std::string > l_tokens;

      while( l_ss ) {
        if( !getline( l_ss, l_next, ' ' ) || l_tokens.size() == 4 )
          break;

        l_tokens.push_back( l_next );
      }

      for( l_i = 1; l_i < 4; l_i++ ) {
        l_pos = l_tokens[l_i].find( "//" );
        l_index = atoi( (l_tokens[l_i].substr( 0, l_pos )).c_str() ) - 1;

        //! Populate face-indices
        this->m_indices.push_back( l_index );
      }
    }
  }

  l_in.close();

  auto l_xmax   = std::max_element( std::begin( l_x ), std::end( l_x ) );
  auto l_xmin   = std::min_element( std::begin( l_x ), std::end( l_x ) );
  l_xcntr = (*l_xmax + *l_xmin) / 2;
  l_lmax = abs( *l_xmax - *l_xmin );

  auto l_ymax   = std::max_element( std::begin( l_y ), std::end( l_y ) );
  auto l_ymin   = std::min_element( std::begin( l_y ), std::end( l_y ) );
  l_ycntr = (*l_ymax + *l_ymin) / 2;
  if( abs( *l_ymax - *l_ymin ) > l_lmax )
    l_lmax = abs( *l_ymax - *l_ymin );

  auto l_zmax   = std::max_element( std::begin( l_z ), std::end( l_z ) );
  auto l_zmin   = std::min_element( std::begin( l_z ), std::end( l_z ) );
  l_zcntr = (*l_zmax + *l_zmin) / 2;
  if( abs( *l_zmax - *l_zmin ) > l_lmax )
    l_lmax = abs( *l_zmax - *l_zmin );

  for( l_i = 0; l_i < m_vertices.size(); l_i += 3 ) {
    m_vertices[l_i]   -= l_xcntr;
    m_vertices[l_i+1] -= l_ycntr;
    m_vertices[l_i+2] -= l_zcntr;

    m_vertices[l_i]   *= 2.0f / l_lmax;
    m_vertices[l_i+1] *= 2.0f / l_lmax;
    m_vertices[l_i+2] *= 2.0f / l_lmax;
  }
}

Geometry::Geometry( const char *i_fileName ) {
  //! parse and load the obj file
  load( i_fileName );

  glGenVertexArrays( 1, &m_VAO );

  glGenBuffers( 1, &m_VBO );
  glGenBuffers( 1, &m_EBO );
  glGenBuffers( 1, &m_NBO );

  glBindVertexArray( m_VAO );

  glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
  glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof( GLfloat ),
                &m_vertices[0], GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ),
                         (GLvoid *) 0 );

  glBindBuffer( GL_ARRAY_BUFFER, m_NBO );
  glBufferData( GL_ARRAY_BUFFER, m_normals.size() * sizeof( GLfloat ),
                &m_normals[0], GL_STATIC_DRAW );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ),
                         (GLvoid *) 0 );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof( GLuint ),
                &m_indices[0], GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
}

Geometry::~Geometry() {
  glDeleteVertexArrays( 1, &m_VAO );

  glDeleteBuffers( 1, &m_VBO );
  glDeleteBuffers( 1, &m_NBO );
  glDeleteBuffers( 1, &m_EBO );
}

void Geometry::draw( const GLuint &i_shaderProgram, const glm::mat4 &i_mtx ) {
  glm::mat4 l_modelView = i_mtx;

  m_uProjection = glGetUniformLocation( i_shaderProgram, "projection" );
  m_uModelView  = glGetUniformLocation( i_shaderProgram, "modelView"  );

  glUniformMatrix4fv( m_uProjection, 1, GL_FALSE, &Window::m_P[0][0] );
  glUniformMatrix4fv( m_uModelView,  1, GL_FALSE, &l_modelView[0][0] );

  glBindVertexArray( m_VAO );
  glDrawElements( GL_TRIANGLES, m_indices.size() * sizeof( GLuint ),
                  GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 );
}

void Geometry::update( const glm::mat4 &i_mtx ) {}


void Group::addChild( Node *i_child ) {
  m_ptrs.push_back( i_child );
}

//! untested: don't use
void Group::removeChild() {
  m_ptrs.pop_back();
}

void Group::draw( const GLuint &i_shaderProgram, const glm::mat4 &i_mtx ) {
  for( std::list< Node * >::iterator l_it = m_ptrs.begin(); l_it != m_ptrs.end(); ++l_it )
    (*l_it)->draw( i_shaderProgram, i_mtx );
}

void Group::update( const glm::mat4 &i_mtx ) {}
