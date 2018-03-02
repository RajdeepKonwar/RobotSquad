#include "Window.h"

#define WINDOW_TITLE "Robot Squad"

//! Shader paths
#define SKYBOX_VERT_SHADER  "SkyBoxShader.vert"
#define SKYBOX_FRAG_SHADER  "SkyBoxShader.frag"
#define ROBOT_VERT_SHADER   "RobotShader.vert"
#define ROBOT_FRAG_SHADER   "RobotShader.frag"
#define CURVE_VERT_SHADER   "CurveShader.vert"
#define CURVE_FRAG_SHADER   "CurveShader.frag"

GLuint   g_curveShader, g_robotShader, g_skyBoxShader;
SkyBox  *g_skyBox;

Node    *g_squadMarch;
Node    *g_ulLimbSwing, *g_urLimbSwing, *g_llLimbSwing, *g_lrLimbSwing;

Node    *g_robotSquad, *g_robotPos[25];
Node    *g_robot, *g_modelMtx;

Node    *g_lAntennaMtx, *g_rAntennaMtx, *g_headMtx;
Node    *g_lEyeballMtx, *g_rEyeballMtx, *g_bodyMtx;
Node    *g_ulLimbMtx, *g_urLimbMtx, *g_llLimbMtx, *g_lrLimbMtx;

Node    *g_antenna, *g_head, *g_eyeball, *g_body, *g_limb;
Curve   *g_curve;

//! Default camera parameters
glm::vec3 g_camPos( -20.0f, 10.0f, 20.0f );  //! e | Position of camera
glm::vec3 g_camLookAt( 0.0f, 0.0f,  0.0f ); //! d | This is where camera looks at
glm::vec3 g_camUp(     0.0f, 1.0f,  0.0f ); //! u | What orientation "up" is

//! Static data members
int       Window::m_width;
int       Window::m_height;
int       Window::m_move  = 0;

float     Window::m_angle = 0.0f;
int       Window::m_swing = 1;

glm::vec3 Window::m_lastPoint( 0.0f, 0.0f, 0.0f );
glm::mat4 Window::m_P;
glm::mat4 Window::m_V;

void Window::initialize_objects() {
  //! SkyBox
  g_skyBox      = new SkyBox;

  //! Squad marching along Bezier curve
  g_squadMarch  = new Transform( glm::mat4( 1.0f ) );

  //! Animation of limbs swinging
  g_ulLimbSwing = new Transform( glm::mat4( 1.0f ) );
  g_urLimbSwing = new Transform( glm::mat4( 1.0f ) );
  g_llLimbSwing = new Transform( glm::mat4( 1.0f ) );
  g_lrLimbSwing = new Transform( glm::mat4( 1.0f ) );

  //! Robot Squad
  g_robotSquad  = new Group;

  //! Robot
  g_robot       = new Group;

  //! Model matrix
  g_modelMtx    = new Transform( glm::mat4( 1.0f ) );

  //! Antennas transformation matrices
  g_lAntennaMtx = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::rotate( glm::mat4( 1.0f ),
                                              -180.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 0.0f, 0.0f, 1.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( -0.2f, 0.0f, 1.0f ) ) );
  g_rAntennaMtx = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( -0.2f, 0.0f, 1.0f ) ) );

  //! Head transformation matrices
  g_headMtx     = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( 0.0f, 0.0f, 1.5f ) ) );

  //! Eyeball  transformation matrices
  g_lEyeballMtx = new Transform( glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.25f ) ) *
                                 glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( -1.3f, -2.5f, 5.5f ) ) );
  g_rEyeballMtx = new Transform( glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.25f ) ) *
                                 glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( 1.3f, -2.5f, 5.5f ) ) );

  //! Body transformation matrices
  g_bodyMtx     = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( 0.0f, 0.0f, 0.0f ) ) );

  //! Limb transformation matrices
  g_ulLimbMtx   = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( -1.3f, 0.0f, 0.0f ) ) );
  g_urLimbMtx   = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( 1.3f, 0.0f, 0.0f ) ) );
  g_llLimbMtx   = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( -0.5f, 0.0f, -1.0f ) ) );
  g_lrLimbMtx   = new Transform( glm::rotate( glm::mat4( 1.0f ),
                                              -90.0f / 180.0f * glm::pi< GLfloat >(),
                                              glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                                 glm::translate( glm::mat4( 1.0f ),
                                                 glm::vec3( 0.5f, 0.0f, -1.0f ) ) );

  //! Part geometries
  g_antenna     = new Geometry( "antenna.obj" );
  g_head        = new Geometry( "head.obj" );
  g_eyeball     = new Geometry( "eyeball.obj" );
  g_body        = new Geometry( "body.obj" );
  g_limb        = new Geometry( "limb.obj" );

  //! Curve1
  Curve::genCurve( glm::vec3( -10.0f,  0.0f, -10.0f ),
                   glm::vec3( -15.0f, -5.0f,   0.0f ),
                   glm::vec3(  20.0f, 10.0f,  20.0f ),
                   glm::vec3(  30.0f, 15.0f, -10.0f ) );

  //! Curve2
  Curve::genCurve( glm::vec3( 30.0f, 15.0f, -10.0f ),
                   glm::vec3( 40.0f, 20.0f, -40.0f ),
                   glm::vec3( 80.0f, 25.0f, -30.0f ),
                   glm::vec3( 70.0f, 30.0f, -50.0f ) );

  //! Curve3
  Curve::genCurve( glm::vec3( 70.0f, 30.0f, -50.0f ),
                   glm::vec3( 60.0f, 35.0f, -70.0f ),
                   glm::vec3( 30.0f, 35.0f, -80.0f ),
                   glm::vec3(  0.0f, 30.0f, -60.0f ) );

  //! Curve4
  Curve::genCurve( glm::vec3(   0.0f, 30.0f, -60.0f  ),
                   glm::vec3( -30.0f, 25.0f, -40.0f  ),
                   glm::vec3( -10.0f, 20.0f, -120.0f ),
                   glm::vec3( -10.0f, 15.0f, -50.0f  ) );

  //! Curve5
  Curve::genCurve( glm::vec3( -10.0f, 15.0f, -50.0f ),
                   glm::vec3( -10.0f, 10.0f,  20.0f ),
                   glm::vec3(  -5.0f,  5.0f, -20.0f ),
                   glm::vec3( -10.0f,  0.0f, -10.0f ) );

  //! Bezier curve
  g_curve = new Curve;

  //! Adding geometries to transformation matrices as node children
  static_cast< Transform * >(g_lAntennaMtx)->addChild( g_antenna );
  static_cast< Transform * >(g_rAntennaMtx)->addChild( g_antenna );
  static_cast< Transform * >(g_headMtx)->addChild( g_head );
  static_cast< Transform * >(g_lEyeballMtx)->addChild( g_eyeball );
  static_cast< Transform * >(g_rEyeballMtx)->addChild( g_eyeball );
  static_cast< Transform * >(g_bodyMtx)->addChild( g_body );
  static_cast< Transform * >(g_ulLimbMtx)->addChild( g_limb );
  static_cast< Transform * >(g_urLimbMtx)->addChild( g_limb );
  static_cast< Transform * >(g_llLimbMtx)->addChild( g_limb );
  static_cast< Transform * >(g_lrLimbMtx)->addChild( g_limb );

  //! Adding transformation matrices as node children to model matrix
  static_cast< Transform * >(g_modelMtx)->addChild( g_lAntennaMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_rAntennaMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_headMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_lEyeballMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_rEyeballMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_bodyMtx );

  //! Adding model matrix as child to robot
  static_cast< Group * >(g_robot)->addChild( g_modelMtx );

  //! The 25 robots' position transformation matrices
  for( int l_i = -2; l_i < 3; l_i++ ) {
    for( int l_j = -2; l_j < 3; l_j++ ) {
      g_robotPos[5*(l_i+2)+(l_j+2)] = new Transform( glm::translate( glm::mat4( 1.0f ),
                                                     glm::vec3( l_j * 3.5f, 0.0f,
                                                                l_i * 3.5f ) ) );

      static_cast< Transform * >(g_robotPos[5*(l_i+2)+(l_j+2)])->addChild( g_robot );
      static_cast< Group * >(g_robotSquad)->addChild( g_robotPos[5*(l_i+2)+(l_j+2)] );
    }
  }

  //! Adding robot squad as child to squad march
  static_cast< Transform * >(g_squadMarch)->addChild( g_robotSquad );

  /** Adding each limb's trans_mtx as child to respective swing trans_mtx
   *  Rerouting above trans_mtx to the model mtx via the swing trans_mtx
   **/
  static_cast< Transform * >(g_ulLimbSwing)->addChild( g_ulLimbMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_ulLimbSwing );

  static_cast< Transform * >(g_urLimbSwing)->addChild( g_urLimbMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_urLimbSwing );

  static_cast< Transform * >(g_llLimbSwing)->addChild( g_llLimbMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_llLimbSwing );

  static_cast< Transform * >(g_lrLimbSwing)->addChild( g_lrLimbMtx );
  static_cast< Transform * >(g_modelMtx)->addChild( g_lrLimbSwing );

  //! Load the shader programs
  g_skyBoxShader  = LoadShaders( SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER );
  g_robotShader   = LoadShaders( ROBOT_VERT_SHADER,  ROBOT_FRAG_SHADER  );
  g_curveShader   = LoadShaders( CURVE_VERT_SHADER,  CURVE_FRAG_SHADER  );
}

//! Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up() {
  delete g_skyBox;
  delete g_curve;
  delete g_robotSquad;
  delete g_robot;
  delete g_modelMtx;

  delete g_squadMarch;
  delete g_ulLimbSwing;
  delete g_urLimbSwing;
  delete g_llLimbSwing;
  delete g_lrLimbSwing;

  for( int l_i = 0; l_i < 25; l_i++ )
    delete g_robotPos[l_i];

  delete g_lAntennaMtx;
  delete g_rAntennaMtx;
  delete g_headMtx;
  delete g_lEyeballMtx;
  delete g_rEyeballMtx;
  delete g_bodyMtx;
  delete g_ulLimbMtx;
  delete g_urLimbMtx;
  delete g_llLimbMtx;
  delete g_lrLimbMtx;

  delete g_antenna;
  delete g_head;
  delete g_eyeball;
  delete g_body;
  delete g_limb;

  glDeleteProgram( g_skyBoxShader );
  glDeleteProgram( g_robotShader  );
  glDeleteProgram( g_curveShader  );
}

GLFWwindow* Window::create_window( int i_width, int i_height ) {
  //! Initialize GLFW
  if( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return NULL;
  }

  //! 4x antialiasing
  glfwWindowHint( GLFW_SAMPLES, 4 );

#ifdef __APPLE__ //! Because Apple hates comforming to standards
  //! Ensure that minimum OpenGL version is 3.3
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

  //! Enable forward compatibility and allow a modern OpenGL context
  glfwWindowHint( GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE                  );
#endif

  //! Create the GLFW window
  GLFWwindow *l_window = glfwCreateWindow( i_width, i_height, WINDOW_TITLE,
                                           nullptr, nullptr );

  //! Check if the window could not be created
  if( !l_window ) {
    fprintf( stderr, "Failed to open GLFW window.\n" );
    fprintf( stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n" );
    glfwTerminate();
    return nullptr;
  }

  //! Make the context of the window
  glfwMakeContextCurrent( l_window );

  //! Set swap interval to 1
  glfwSwapInterval( 1 );

  //! Get the width and height of the framebuffer to properly resize the window
  glfwGetFramebufferSize( l_window, &i_width, &i_height );

  //! Call the resize callback to make sure things get drawn immediately
  Window::resize_callback( l_window, i_width, i_height );

  return l_window;
}

void Window::resize_callback( GLFWwindow* i_window, int i_width, int i_height ) {
#ifdef __APPLE__
  glfwGetFramebufferSize( i_window, &i_width, &i_height );  //! In case your Mac has a retina display
#endif

  Window::m_width   = i_width;
  Window::m_height  = i_height;

  //! Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
  glViewport( 0, 0, i_width, i_height );

  if( i_height > 0 ) {
    Window::m_P = glm::perspective( 45.0f, (float) i_width / (float) i_height,
                                    0.1f, 2000.0f );
    Window::m_V = glm::lookAt( g_camPos, g_camLookAt, g_camUp );
  }
}

void Window::idle_callback() {
  //! Switch swing direction
  if( Window::m_angle <= -40.0f || Window::m_angle >= 40.0f )
    Window::m_swing *= -1;

  //! Update swing angle with direction
  Window::m_angle = Window::m_angle + (float ) Window::m_swing;

  //! Upper left limb swing
  glm::mat4 l_tMtx  = glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, 0.7f, 0.0f ) ) * 
                      glm::rotate( glm::mat4( 1.0f ),
                                   Window::m_angle / 180.0f * glm::pi< GLfloat >(),
                                   glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                      glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, -0.7f, 0.0f ) );
  static_cast< Transform * >(g_ulLimbSwing)->update( l_tMtx );

  //! Upper right limb swing
  l_tMtx            = glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, 0.7f, 0.0f ) ) * 
                      glm::rotate( glm::mat4( 1.0f ),
                                   Window::m_angle / 180.0f * glm::pi< GLfloat >(),
                                   glm::vec3( -1.0f, 0.0f, 0.0f ) ) *
                      glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, -0.7f, 0.0f ) );
  static_cast< Transform * >(g_urLimbSwing)->update( l_tMtx );

  //! Lower left limb swing
  l_tMtx            = glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, -0.7f, 0.0f ) ) * 
                      glm::rotate( glm::mat4( 1.0f ),
                                   Window::m_angle / 180.0f * glm::pi< GLfloat >(),
                                   glm::vec3( -1.0f, 0.0f, 0.0f ) ) *
                      glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, 0.7f, 0.0f ) );
  static_cast< Transform * >(g_llLimbSwing)->update( l_tMtx );

  //! Lower right limb swing
  l_tMtx            = glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, -0.7f, 0.0f ) ) * 
                      glm::rotate( glm::mat4( 1.0f ),
                                   Window::m_angle / 180.0f * glm::pi< GLfloat >(),
                                   glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
                      glm::translate( glm::mat4( 1.0f ),
                                      glm::vec3( 0.0f, 0.7f, 0.0f ) );
  static_cast< Transform * >(g_lrLimbSwing)->update( l_tMtx );

  //! Marching along Bezier curve
  static_cast< Transform * >(g_squadMarch)->update(
              glm::translate( glm::mat4( 1.0f ),
                              glm::vec3( Curve::m_vertices[Curve::m_cnt],
                                         Curve::m_vertices[Curve::m_cnt+1],
                                         Curve::m_vertices[Curve::m_cnt+2] ) ) );

  //! Updating/Resetting march-counter
  if( Curve::m_cnt < (Curve::m_vertices.size() - 3.0f) )
    Curve::m_cnt += 3;
  else
    Curve::m_cnt  = 0;
}

void Window::display_callback( GLFWwindow* i_window ) {
  //! Clear the color and depth buffers
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDepthMask( GL_FALSE );

  //! Use SkyBoxShader
  glUseProgram( g_skyBoxShader );
  g_skyBox->draw( g_skyBoxShader );

  glDepthMask( GL_TRUE );

  //! Use RobotShader
  glUseProgram( g_robotShader );
  g_squadMarch->draw( g_robotShader, Window::m_V );

  //! Use CurveShader
  glUseProgram( g_curveShader );
  g_curve->draw( g_curveShader );

  //! Gets events, including input such as keyboard and mouse or window resizing
  glfwPollEvents();

  //! Swap buffers
  glfwSwapBuffers( i_window );

  //! Refresh view matrix with new camera position every display callback
  Window::m_V = glm::lookAt( g_camPos, g_camLookAt, g_camUp );
}

void Window::key_callback( GLFWwindow *i_window, int i_key, int i_scancode,
                           int i_action, int i_mods ) {
  if( i_action == GLFW_PRESS || i_action == GLFW_REPEAT ) {
    switch( i_key ) {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose( i_window, GL_TRUE );
        break;
    }
  }
}

glm::vec3 trackBallMapping( glm::vec3 i_point ) {
  glm::vec3 l_v;
  float     l_d;

  l_v.x = (2.0f * i_point.x - (float) Window::m_width)  / (float) Window::m_width;
  l_v.y = ((float) Window::m_height - 2.0f * i_point.y) / (float) Window::m_height;
  l_v.z = 0.0f;

  l_d   = glm::length( l_v );
  l_d   = (l_d < 1.0f) ? l_d : 1.0f;
  l_v.z = sqrtf( 1.001f - powf( l_d, 2.0f ) );
  glm::normalize( l_v );

  return l_v;
}

void Window::cursor_pos_callback( GLFWwindow *i_window, double i_xPos,
                                  double i_yPos ) {
  glm::vec3 l_direction, l_currPoint, l_rotAxis;
  float     l_vel, l_rotAngle;

  switch( Window::m_move ) {
    case 1:   //! Rotation
      l_currPoint = trackBallMapping( glm::vec3( (float) i_xPos, (float) i_yPos,
                                      0.0f ) );
      l_direction = l_currPoint - Window::m_lastPoint;
      l_vel       = glm::length( l_direction );

      if( l_vel > 0.0001f ) {
        l_rotAxis   = glm::cross( Window::m_lastPoint, l_currPoint );
        l_rotAngle  = l_vel * 0.01f;

        //! Update camera position
        glm::vec4 l_tmp = glm::rotate( glm::mat4( 1.0f ), -l_rotAngle, l_rotAxis )
                          * glm::vec4( g_camPos, 1.0f );
        g_camPos  = glm::vec3( l_tmp.x, l_tmp.y, l_tmp.z );
      }

      break;

    case 2:   //! Panning (Not implemented)
      break;
  }
}

void Window::mouse_button_callback( GLFWwindow *i_window, int i_button,
                                    int i_action, int i_mods ) {
  double l_xPos, l_yPos;

  if( i_action == GLFW_PRESS ) {
    switch( i_button ) {
      case GLFW_MOUSE_BUTTON_LEFT:
        Window::m_move  = 1;

        glfwGetCursorPos( i_window, &l_xPos, &l_yPos );
        Window::m_lastPoint = trackBallMapping( glm::vec3( (float) l_xPos,
                                                           (float) l_yPos,
                                                           0.0f ) );
        break;

      case GLFW_MOUSE_BUTTON_RIGHT:
        Window::m_move  = 2;
        break;
    }
  }

  else if( i_action == GLFW_RELEASE ) {
    Window::m_move  = 0;
  }
}

void Window::scroll_callback( GLFWwindow *i_window, double i_xOffset,
                              double i_yOffset ) {
  //! Avoid scrolling out of cubemap
  if( ((int) i_yOffset == -1) && (g_camPos.x > 900.0f || g_camPos.y > 900.0f
                                                      || g_camPos.z > 900.0f) )
    return;

  //! Reposition camera to new location
  glm::vec3 l_dir = g_camPos - g_camLookAt;
  glm::normalize( l_dir );
  g_camPos  -= l_dir * (float) i_yOffset * 0.1f;
}
