#include "NGLScene.h"
#include <ngl/Texture.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>
#include <QMouseEvent>
#include "MultiBufferIndexVAO.h"


NGLScene::NGLScene()
{
  setTitle( "Bump and Normal Mapping" );
}


NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}


void NGLScene::resizeGL( int _w, int _h )
{
  m_projection=ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.1f, 200.0f );

  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

void NGLScene::buildVAO()
{
  std::array<float,72> verts ={{
      -1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f, // Front
      -1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f, // Back
       1.0f, -1.0f, -1.0f,   1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f, -1.0f, // Right
      -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f, -1.0f, // Left
      -1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f, // Top
      -1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f, // Bottom
  }};

    // Tangents
   std::array<float,72> tangents = {{
       1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f, // Front
      -1.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f, // Back
       0.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f, // Right
       0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f, // Left
       1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f, // Top
       1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  0.0f, // Bottom
    }};

    // Bitangents
    std::array<float,72> bitangents = {{
       0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f, // Front
       0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f, // Back
       0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f, // Right
       0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, -1.0f,  0.0f, // Left
       0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f, // Top
       0.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f, -1.0f, // Bot
    }};
    std::array<float,48> uv = {{
       0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, // Front
       1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // Back
       1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, // Right
       0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f, // Left
       0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f, // Top
       0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, // Bottom
    }};
    std::array<GLshort,36> indices = {{
      0 , 1 , 2 ,    0 , 3 , 1 , // Front
      4 , 6 , 5 ,    4 , 5 , 7 , // Back
      8 , 9 , 10,    8 , 11, 9 , // Right
      12, 14, 13,    12, 13, 15, // Left
      16, 18, 17,    16, 17, 19, // Top
      20, 21, 22,    20, 23, 21, // Bottom
}};
    m_vao=ngl::vaoFactoryCast<MultiBufferIndexVAO>(ngl::VAOFactory::createVAO("multiBufferIndexVAO",GL_TRIANGLES));
    m_vao->bind();
    m_vao->setData(MultiBufferIndexVAO::VertexData(verts.size()*sizeof(float),verts[0]));
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->setData(MultiBufferIndexVAO::VertexData(tangents.size()*sizeof(float),tangents[0]));
    m_vao->setVertexAttributePointer(1,3,GL_FLOAT,0,0);
    m_vao->setData(MultiBufferIndexVAO::VertexData(bitangents.size()*sizeof(float),bitangents[0]));
    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);
    m_vao->setData(MultiBufferIndexVAO::VertexData(uv.size()*sizeof(float),uv[0]));
    m_vao->setVertexAttributePointer(3,2,GL_FLOAT,0,0);
    m_vao->setIndices(sizeof(indices),&indices[0], GL_UNSIGNED_SHORT);
    m_vao->setNumIndices(indices.size());

    m_vao->unbind();
}


constexpr auto shaderProgram = "NormalMap";


void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::instance();
  // uncomment this line to make ngl less noisy with debug info
  // ngl::NGLInit::instance()->setCommunicationMode( ngl::CommunicationMode::NULLCONSUMER);
  glClearColor( 0.4f, 0.4f, 0.4f, 1.0f ); // Grey Background
  // enable depth testing for drawing
  glEnable( GL_DEPTH_TEST );
  // enable multisampling for smoother drawing
  glEnable( GL_MULTISAMPLE );
  // now to load the shader and set the values
  // grab an instance of shader manager
  m_text.reset(new  ngl::Text(QFont("Arial",18)));
  m_text->setScreenSize(width(),height());

  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  // we are creating a shader called PBR to save typos
  // in the code create some constexpr
  shader->loadShader(shaderProgram,"shaders/NormalMapVertex.glsl","shaders/NormalMapFragment.glsl");
  // and make it active ready to load values
  ( *shader )[ shaderProgram ]->use();
 // We now create our view matrix for a static camera
  ngl::Vec3 from( 0.0f, 2.0f, 4.0f );
  ngl::Vec3 to( 0.0f, 0.0f, 0.0f );
  ngl::Vec3 up( 0.0f, 1.0f, 0.0f );
  // now load to our new camera
  m_view=ngl::lookAt(from,to,up);
  m_lightPos.set( 0.0, 2.0f, 2.0f ,1.0f);

  // register our new Factory to draw the VAO
  ngl::VAOFactory::registerVAOCreator("multiBufferIndexVAO", MultiBufferIndexVAO::create);
  ngl::VAOFactory::listCreators();
  buildVAO();

  // load and set a texture for the colour
  ngl::Texture t("textures/diffuse.png");
  t.setMultiTexture(1);
  m_diffuse=t.setTextureGL();
  shader->setUniform("diffuseSampler",1);

  // mip map the textures
  glGenerateMipmap(GL_TEXTURE_2D);
  // now one for the depth map
  ngl::Texture depth("textures/depth.png");
  depth.setMultiTexture(2);
  m_depth=depth.setTextureGL();
  shader->setUniform("depthSampler",2);

  // mip map the textures
  glGenerateMipmap(GL_TEXTURE_2D);
  // this is our normal map
  ngl::Texture normal("textures/normal.png");
  normal.setMultiTexture(0);
  m_normal=normal.setTextureGL();
  shader->setUniform("normalSampler",0);
  // mip map the textures
  glGenerateMipmap(GL_TEXTURE_2D);
  shader->setUniform("type",1);
  shader->setUniform("show_tex",1);
  shader->setUniform("depth_scale",0.01f);
  shader->setUniform("num_layers",32.0f);

  // now floor with check shader

  ngl::VAOPrimitives::instance()->createTrianglePlane("floor",20,20,1,1,ngl::Vec3::up());
  shader->use(ngl::nglCheckerShader);
  shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);
  shader->setUniform("checkOn",true);
  shader->setUniform("lightPos",m_lightPos.toVec3());
  shader->setUniform("colour1",0.9f,0.9f,0.9f,1.0f);
  shader->setUniform("colour2",0.6f,0.6f,0.6f,1.0f);
  shader->setUniform("checkSize",60.0f);


}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  shader->use(shaderProgram);
  struct transform
  {
    ngl::Mat4 MVP;
    ngl::Mat4 normalMatrix;
    ngl::Mat4 M;
  };

   transform t;
   t.M=m_view*m_mouseGlobalTX;

   t.MVP=m_projection*t.M;
   t.normalMatrix=t.M;
   t.normalMatrix.inverse().transpose();
   shader->setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
   shader->setUniform("type",m_type);
   ngl::msg->addMessage(fmt::format("type is {0}",m_type));
}

void NGLScene::paintGL()
{
  glViewport( 0, 0, m_win.width, m_win.height );
  // clear the screen and depth buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX( m_win.spinXFace );
  rotY.rotateY( m_win.spinYFace );
  // multiply the rotations
  m_mouseGlobalTX = rotX * rotY;
  // add the translations
  m_mouseGlobalTX.m_m[ 3 ][ 0 ] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[ 3 ][ 1 ] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[ 3 ][ 2 ] = m_modelPos.m_z;
  // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives* prim = ngl::VAOPrimitives::instance();
  // draw
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  shader->use(shaderProgram);
  // need to re-activate the textures as we also use texture when using text
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,m_normal);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D,m_diffuse);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D,m_depth);

  loadMatricesToShader();
  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();

  { // draw floor
    // grab an instance of the shader manager

    shader->use(ngl::nglCheckerShader);
    ngl::Mat4 tx;
    tx.translate(0.0f,-1.0f,0.0f);
    ngl::Mat4 MVP=m_projection*m_view*m_mouseGlobalTX*tx;
    ngl::Mat3 normalMatrix=m_view*m_mouseGlobalTX;
    normalMatrix.inverse().transpose();
    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
    if(m_transformLight)
    {
      shader->setUniform("lightPosition",(m_mouseGlobalTX*m_lightPos).toVec3());
    }
    prim->draw("floor");

  }
    // draw text
    {
      std::array<char *,5>modes={{"No bump mapping",
      "Normal mapping",
      "Parallax mapping",
      "Steep parallax mapping",
      "Parallax occlusion mapping"}};
      m_text->setColour(1.0f,1.0f,1.0f);

      m_text->renderText(10,20,QString("Mode :- %1").arg(modes[m_type]) );
    }

}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent( QKeyEvent* _event )
{
  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch ( _event->key() )
  {
    // escape key to quit
    case Qt::Key_Escape:
      QGuiApplication::exit( EXIT_SUCCESS );
      break;
// turn on wirframe rendering
#ifndef USINGIOS_
    case Qt::Key_W:
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      break;
    // turn off wire frame
    case Qt::Key_S:
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      break;
#endif
    // show full screen
    case Qt::Key_F:
      showFullScreen();
      break;
    // show windowed
    case Qt::Key_N:
      showNormal();
      break;
    case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());
    break;

    case Qt::Key_L :
    m_transformLight^=true;
    break;

  case Qt::Key_0 : m_type = 0; break;
  case Qt::Key_1 : m_type = 1; break;
  case Qt::Key_2 : m_type = 2; break;
  case Qt::Key_3 : m_type = 3; break;
  case Qt::Key_4 : m_type = 4; break;


    default:
      break;
  }
  update();
}
