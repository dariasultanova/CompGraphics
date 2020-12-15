#ifndef SCENE_HPP
#define SCENE_HPP

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QThread>


class Scene : public QOpenGLWidget
{
  Q_OBJECT

public:
  Scene(QWidget *parent = nullptr);
  ~Scene() override;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void keyPressEvent(QKeyEvent * event) override;
  void mousePressEvent(QMouseEvent *pe) override;
  void mouseReleaseEvent(QMouseEvent * pe) override;
  void mouseMoveEvent(QMouseEvent *pe) override;


private:
  GLdouble xRot_;
  GLdouble yRot_;
  GLdouble nScale_ = 1.0;

  QPoint mousePosPtr_;
  QTimer * timer;
  GLboolean isAnmationRunning_ = GL_FALSE;


  // SCENE CONSTANTS
  const GLdouble stageZero_ = -1.0;
  // LIGHTING //
  GLfloat lightPos_[4] = {1.f, 1.f, 1.f, 1.f};
  const GLfloat lightColour_[4] = {1.f, 1.f, 1.f, 1.f};
  const GLfloat noLighting_[4] = {0.f, 0.f, 0.f, 1.f};
  GLfloat ambient[4] = { 0.f, 0.f, 0.f, 1.0f };

  // MATERIALS //
  // BESIER SURFACE
  GLfloat mat_bezier[4] = { 0.0f, 0.6f, 0.8f, 1.0f };//blue
  // MATH OBJECT
  GLfloat mat_mathObj[4] = { 1.0f, 1.0f, 0.0f, 1.f }; //
  // ARRAY OBJECT
  GLfloat mat_vertexObj[4] = { 0.f/255.f, 51.f/255.f, 0.f/255.f, 1.f  };

  // SHINE COLOUR //
  GLfloat mat_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat mat_black[4] = { 1.0f, 1.0f, 0.0f, 1.f };
  GLfloat mat_shininess[1] = { 50.0 };

  GLfloat mat_red_[4] = { 1.f, 0.f, 0.f, 1.f };
  GLfloat mat_green_[4] = { 0.f, 1.f, 0.f, 1.0f };
  GLfloat mat_blue_[4] = { 0.f, 0.f, 1.f, 1.f };
  GLfloat mat_orange_[4] = { 1.0f, 0.5f, 0.1f, 1.f };
  GLfloat black_[4] = { 0.0f, 0.0f, 0.0f, 1.f };
  GLfloat white_[4] = { 1.0f, 1.0f, 1.0f, 1.f };

  GLdouble transformLowerLim_;
  GLdouble transformUpperLim_;

  const static GLint frameCount_ = 60;

  // SCENE CONTROL VERIABLES //
  GLboolean drawSurface_;
  GLboolean drawCone_;

  GLdouble transformCount_;
  GLint currFrame_ = 0;  //for bezier and VAO animation
  GLboolean isAnimAscanding_ = GL_TRUE;


  // BEZIER SURFACE OPTIONS //
  const static GLuint nurbGridSize_ = 5;
  const GLdouble mathExp_ = 0.;
  const GLdouble standDev_ = 0.35;
  const GLdouble surfLims_[2] = {-0.5, 0.5};
  GLdouble nurbPoints_[frameCount_][nurbGridSize_][nurbGridSize_][3];
  const GLdouble nurbSphereRad_ = 0.5;
  GLdouble nurbPointsA_[nurbGridSize_][nurbGridSize_][3] = {
    { {0., 0., 0.0}, {.3, 0., 0.0}, {.6, 0., 0.0}, {.9, 0., 0.0}, {1.2, 0., 0.0} },
    { {0., 0., 0.3}, {.3, 0., 0.3}, {.6, 0., 0.3}, {.9, 0., 0.3}, {1.2, 0., 0.3} },
    { {0., 0., 0.6}, {.3, 0., 0.6}, {.6, 0., 0.6}, {.9, 0., 0.6}, {1.2, 0., 0.6} },
    { {0., 0., 0.9}, {.3, 0., 0.9}, {.6, 0., 0.9}, {.9, 0., 0.9}, {1.2, 0., 0.9} },
    { {0., 0., 1.2}, {.3, 0., 1.2}, {.6, 0., 1.2}, {.9, 0., 1.2}, {1.2, 0., 1.2} }
  };
  GLdouble nurbPointsB_[nurbGridSize_][nurbGridSize_][3] = {
    { {.6, -.5, 0.0}, {.3, -.3, 0.0}, {.6, 0., 0.0}, {.9, -.3, 0.0}, {0.6, -.5, 0.0} },
    { {.6, -.5, 0.3}, {.3, -.3, 0.3}, {.6, 0., 0.3}, {.9, -.3, 0.3}, {0.6, -.5, 0.3} },
    { {.6, -.5, 0.6}, {.3, -.3, 0.6}, {.6, 0., 0.6}, {.9, -.3, 0.6}, {0.6, -.5, 0.6} },
    { {.6, -.5, 0.9}, {.3, -.3, 0.9}, {.6, 0., 0.9}, {.9, -.3, 0.9}, {0.6, -.5, 0.9} },
    { {.6, -.5, 1.2}, {.3, -.3, 1.2}, {.6, 0., 1.2}, {.9, -.3, 1.2}, {0.6, -.5, 1.2} }
  };

  // BI_CONE OPTIONS //
  const static GLuint detail_ = 64;
  const GLdouble curveLims_[2] = { 0.0, -1.0 };
  GLdouble curviness_ = curveLims_[0];
  GLdouble curveStep_;
  GLdouble biconeRad_ = 0.4;
  GLdouble biconeHeight_ = biconeRad_*2.;
  // alt
  GLUquadric * biconeQuadrcs_[detail_];

  // VERTEX ARRAY OPTIONS //
  const static GLint pointArrSize_ = 24*3;
  constexpr const static GLdouble CUBE_RADIUS = 0.245;
  GLdouble cubeVertexArrs[frameCount_][pointArrSize_];
  GLdouble cubeNormalsArrs[frameCount_][pointArrSize_];

  GLdouble cubeVertexArrA_[pointArrSize_] = {
    // upper face
    -CUBE_RADIUS, CUBE_RADIUS, CUBE_RADIUS, // 0
    CUBE_RADIUS, CUBE_RADIUS, CUBE_RADIUS,
    CUBE_RADIUS, CUBE_RADIUS, -CUBE_RADIUS,
    -CUBE_RADIUS, CUBE_RADIUS, -CUBE_RADIUS,
    // lower face
    -CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,  // 4
    CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,
    CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    -CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    //front face
    -CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,  // 8
    CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,
    CUBE_RADIUS, CUBE_RADIUS, CUBE_RADIUS,
    -CUBE_RADIUS, CUBE_RADIUS, CUBE_RADIUS,
    // back face
    -CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS, // 12
    CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    CUBE_RADIUS, CUBE_RADIUS, -CUBE_RADIUS,
    -CUBE_RADIUS, CUBE_RADIUS, -CUBE_RADIUS,
    // left face
    -CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,  // 16
    -CUBE_RADIUS, CUBE_RADIUS, CUBE_RADIUS,
    -CUBE_RADIUS, CUBE_RADIUS, -CUBE_RADIUS,
    -CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    // right face
    CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,  // 20
    CUBE_RADIUS, CUBE_RADIUS, CUBE_RADIUS,
    CUBE_RADIUS, CUBE_RADIUS, -CUBE_RADIUS,
    CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS
  };

  GLdouble normalArrA_[pointArrSize_] = {
    // upper face
    0., 1., 0.,  // 0
    0., 1., 0.,
    0., 1., 0.,
    0., 1., 0.,
    // lower face
    0., -1., 0.,  // 4
    0., -1., 0.,
    0., -1., 0.,
    0., -1., 0.,
    // front face
    0., 0., 1,  // 8
    0., 0., 1,
    0., 0., 1,
    0., 0., 1,
    // back face
    0., 0., -1,  // 12
    0., 0., -1,
    0., 0., -1,
    0., 0., -1,
    // left face
    -1., 0., 0.,  // 16
    -1., 0., 0.,
    -1., 0., 0.,
    -1., 0., 0.,
    // right face
    1., 0., 0.,  //20
    1., 0., 0.,
    1., 0., 0.,
    1., 0., 0.,
  };


  GLdouble cubeVertexArrB_[pointArrSize_] = {
    // upper face
    -0., CUBE_RADIUS, 0., // 0
    0, CUBE_RADIUS, 0.,
    0., CUBE_RADIUS, -0.,
    -0., CUBE_RADIUS, -0.,
    // lower face
    -CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,  // 4
    CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,
    CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    -CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    //front face
    -CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,  // 8
    CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,
    0., CUBE_RADIUS, 0.,
    -0., CUBE_RADIUS, 0.,
    // back face
    -CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS, // 12
    CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,
    0., CUBE_RADIUS, -0.,
    -0., CUBE_RADIUS, -0.,
    // left face
    -CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,
    -0., CUBE_RADIUS, 0.,
    -0., CUBE_RADIUS, -0.,
    -CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,  // 16
    // right face
    CUBE_RADIUS, -CUBE_RADIUS, CUBE_RADIUS,
    0., CUBE_RADIUS, 0.,
    0., CUBE_RADIUS, -0.,
    CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS,  // 20
  };

  GLdouble normalArrB_[pointArrSize_] = {
    // upper face
    0., 1., 0.,  // 0
    0., 1., 0.,
    0., 1., 0.,
    0., 1., 0.,
    // lower face
    0., -1., 0.,  // 4
    0., -1., 0.,
    0., -1., 0.,
    0., -1., 0.,
    // front face
    0., 2./5.*CUBE_RADIUS, 4./5.*CUBE_RADIUS,  // 8
    0., 2./5.*CUBE_RADIUS, 4./5.*CUBE_RADIUS,
    0., 2./5.*CUBE_RADIUS, 4./5.*CUBE_RADIUS,
    0., 2./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS,
    // back face
    0., 2./5.*CUBE_RADIUS, -4./5.*CUBE_RADIUS,  // 12
    0., 2./5.*CUBE_RADIUS, -4./5.*CUBE_RADIUS,
    0., 2./5.*CUBE_RADIUS, -4./5.*CUBE_RADIUS,
    0., 2./5.*CUBE_RADIUS, -4./5.*CUBE_RADIUS,
    // left face
    -4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,  // 16
    -4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,
    -4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,
    -4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,
    // right face
    4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,  //20
    4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,
    4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,
    4./5.*CUBE_RADIUS, 2./5.*CUBE_RADIUS, 0.,
  };


  GLdouble colorArr[pointArrSize_] = {
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
    0., 1., 1.,
  };


  GLuint elementArr[pointArrSize_] ={
    // upper face
    0, 1, 2, 3,
    // lower face
    7, 6, 5, 4,
    //front face
    8, 9, 10, 11,
    // back face
    15, 14, 13, 12,
    // left face
    16, 17, 18, 19,
    // right face
    23, 22, 21, 20
  };

  // DRAWING METHODS
  void drawAxis();
  void drawBezierSurface(GLdouble x, GLdouble y, GLdouble z);
  void drawBiconeAlt(GLdouble x, GLdouble y, GLdouble z, GLdouble h, GLdouble rad, GLdouble curviness, GLuint detail);

  // SCENE CONTROL METHODS
  void resetRotation();
  void moveLightUp();
  void moveLightDown();
  void moveLightLeft();
  void moveLightRight();
  void moveLightForth();
  void moveLightBack();


  // ADDITIONAL FUNCTIONS //
  GLdouble normDist(const GLdouble x, const GLdouble exp, const GLdouble dev);
  GLdouble radFuncPlynomial(const GLdouble arg, const GLdouble param);
  GLdouble radFuncSine(const GLdouble arg, const GLdouble param);
  GLdouble radFuncPlum(const GLdouble arg, const GLdouble param);

private slots:
  void playAnimation();
};

#endif // SCENE_HPP
