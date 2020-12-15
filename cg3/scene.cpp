#define STB_IMAGE_IMPLEMENTATION
#include "scene.hpp"
#include "stb_image.hpp"

#include <cmath>
#include <vector>
#include <iostream>

Scene::Scene(QWidget *parent):
  xRot_(25.0),
  yRot_(-45.0),
  transformLowerLim_(0.0),
  transformUpperLim_(1.0),
  drawSurface_(GL_TRUE),
  drawCone_(GL_TRUE),
  transformCount_(transformLowerLim_)
{
  // FILLING THE NURB SURFACE BASE PIONTS //
  GLdouble coeff;
  for (GLuint f = 0; f < frameCount_; f++){
    coeff = static_cast<GLdouble>(f) / (frameCount_ - 1);
    // NURB surface arrays initialization
    for (GLuint i = 0; i < nurbGridSize_; i++){
      for (GLuint j = 0; j < nurbGridSize_; j++){
        for (GLuint k = 0; k < 3; k++){
          nurbPoints_[f][i][j][k] = nurbPointsA_[i][j][k] + coeff * (nurbPointsB_[i][j][k] - nurbPointsA_[i][j][k]);
        }
      }
    }
    // VAO arrays initialization
    for (GLuint k = 0; k < pointArrSize_; k++){
      cubeVertexArrs[f][k] = cubeVertexArrA_[k] + coeff * (cubeVertexArrB_[k] - cubeVertexArrA_[k]);
      cubeNormalsArrs[f][k] = normalArrA_[k] + coeff * (normalArrB_[k] - normalArrA_[k]);
    }
  }

  curveStep_ = (curveLims_[1] - curveLims_[0]) / frameCount_;

}

Scene::~Scene()
{}

void Scene::initializeGL(){
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glEnable (GL_DEPTH_TEST);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

}

void Scene::resizeGL(int w, int h){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLdouble ratio = static_cast<GLdouble>(h) / static_cast<GLdouble>(w);

  if (w >= h)
     // параметры видимости ортогональной проекции
     glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 2.);
  else
     glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);

  glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void Scene::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotated(xRot_, 1.0, 0.0, 0.0);
  glRotated(yRot_, 0.0, 1.0, 0.0);

  glScaled(nScale_, nScale_, nScale_);

  drawAxis();
  glPopMatrix();

  if (drawSurface_){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &mat_bezier[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mat_specular[0]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[0]);
    drawBezierSurface(-0.6, -0.15, -0.85);
  }

  if (drawCone_){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &mat_mathObj[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mat_specular[0]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[0]);
    //drawBicone(0., 0., 0., 0.3, 0.2, curviness_, 32);
    drawBiconeAlt(0., -0.1, 0., biconeHeight_, biconeRad_, curviness_, detail_);
  }

  //FIAT LUX!
  glPushMatrix();
  glLoadIdentity();

  glRotated(xRot_, 1.0, 0.0, 0.0);
  glRotated(yRot_, 0.0, 1.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, &lightPos_[0]);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, &lightColour_[0]);
  glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0]);
  glLightfv(GL_LIGHT0, GL_SPECULAR, &lightColour_[0]);
  glPopMatrix();
}


void Scene::drawAxis(){
    unsigned int texture;
    int width, heigh, cnt;
    unsigned char *data  = stbi_load("img.png", &width, &heigh, &cnt, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,1,1);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLUquadric* quadObj;
    quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GL_TRUE);

    //wings
    //left
    glScalef(1.0, 1.0, 1.7);
    glTranslatef(-0.45, -0.2, -0.2);
    gluSphere(quadObj, 0.15, 100, 10);
    glTranslatef(0.45, 0.2, 0.2);

    //right
    glTranslatef(0.45, -0.2, -0.2);
    gluSphere(quadObj, 0.15, 100, 10);
    glTranslatef(-0.45, 0.2, 0.2);

    //body
    glTranslatef(0.0, -0.2, -0.2);
    glScalef(1.3, 0.85, 1.0);
    gluSphere(quadObj, 0.4, 100, 50);
    glTranslatef(0.0, 0.2, 0.2);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glFlush();

    //bow tie
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &black_[0]);
    glTranslatef(0.0, 0.0, 0.27);
    glutSolidCube(0.1);
    glTranslatef(0.0, 0.0, -0.27);

    glTranslatef(-0.18, 0.0, 0.27);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 1.0, 1.0);
    glutSolidCone(0.1, 0.2, 15, 15);
    glRotatef(-90, 0, 1, 0);
    glTranslatef(0.18, 0.0, -0.27);

    glTranslatef(0.18, 0.0, 0.27);
    glRotatef(-90, 0, 1, 0);
    glutSolidCone(0.1, 0.2, 15, 15);
    glRotatef(90, 0, 1, 0);
    glTranslatef(-0.18, 0.0, -0.27);
    glScalef(1.0, 1.0, 3.33);

    //eyes
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &white_[0]);

    glScalef(1.0, 1.2, 1.0);

    glTranslatef(-0.13, 0.35, 0.1);
    glutSolidSphere(0.09, 35, 35);
    glTranslatef(0.13, -0.35, -0.1);

    glTranslatef(0.13, 0.35, 0.1);
    glutSolidSphere(0.09, 35, 35);
    glTranslatef(-0.13, -0.35, -0.1);

    glColor4f(0.5f, 0.5f, 1.0f, 0.6f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &black_[0]);

    glTranslatef(-0.135, 0.355, 0.12);
    glutSolidSphere(0.078, 35, 35);
    glTranslatef(0.135, -0.355, -0.12);

    glTranslatef(0.135, 0.355, 0.12);
    glutSolidSphere(0.078, 35, 35);
    glTranslatef(-0.135, -0.355, -0.12);

    //beak
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &mat_orange_[0]);
    glScalef(1.8, 0.55, 2.5);
    glTranslatef(0, 0.33, 0.04);
    glutSolidSphere(0.1, 35, 35);
    glTranslatef(0, -0.33, -0.04);

    //axis
//    glBegin(GL_LINES);

//    glColor4f(1.0f, 0.5f, 0.5f, 0.6f);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &mat_red_[0]);
//    glVertex3d(-1.0, stageZero_, stageZero_);
//    glVertex3d(1.0, stageZero_, stageZero_);

//    glColor4f(0.5f, 1.0f, 0.5f, 0.6f);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &mat_green_[0]);
//    glVertex3d(stageZero_, -1.0, stageZero_);
//    glVertex3d(stageZero_, 1.0, stageZero_);

//    glColor4f(0.5f, 0.5f, 1.0f, 0.6f);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &mat_blue_[0]);
//    glVertex3d(stageZero_, stageZero_, -1.0);
//    glVertex3d(stageZero_, stageZero_, 1.0);

//    glEnd();
}

void Scene::drawBezierSurface(GLdouble x, GLdouble y, GLdouble z){
  glDisable(GL_CULL_FACE);
  glEnable(GL_MAP2_VERTEX_3);
  glPushMatrix();
  glLoadIdentity();
  glRotated(xRot_, 1.0, 0.0, 0.0);
  glRotated(yRot_, 0.0, 1.0, 0.0);
  glScaled(nScale_+0.5, nScale_+0.5, nScale_+0.5);
  glTranslated(x, y, z);

  glMap2d(GL_MAP2_VERTEX_3, 0, 1, nurbGridSize_*3, 5, 0, 1, 3, 5, &nurbPoints_[currFrame_][0][0][0]);
  glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);
  glEvalMesh2(GL_FILL, 0, 30, 0, 30);

  glPopMatrix();
  glDisable(GL_MAP2_VERTEX_3);
  glEnable(GL_CULL_FACE);
}

void Scene::drawBiconeAlt(GLdouble x, GLdouble y, GLdouble z, GLdouble h, GLdouble rad, GLdouble curviness, GLuint detail){
  if (detail < 4 ){
    std::cerr << "Too low detalization. Abort drawing.\n";
    return;
  }

  glPushMatrix();
  glLoadIdentity();

  glRotated(xRot_, 1.0, 0.0, 0.0);
  glRotated(yRot_, 0.0, 1.0, 0.0);
  // scaling
  glScaled(nScale_, nScale_, nScale_);
  glTranslated(x, y, z);
  glRotated(-90., 1., 0., 0.);

  std::vector<GLdouble> radiuses(detail);
  for (GLuint i = 0; i < detail; i++){
    radiuses[i] = rad * radFuncPlum(static_cast<GLdouble>(i) / (detail-1), curviness);
  }

  for (GLuint i = detail-1; i > 0; i--){
    biconeQuadrcs_[i] = gluNewQuadric();
    gluCylinder(biconeQuadrcs_[i], radiuses[i], radiuses[i-1], h / detail, static_cast<GLint>(detail), 1);
    glTranslated(0., 0., h / detail);
  }

  glPopMatrix();
}

// SCENE CONTROL METHODS //
void Scene::resetRotation(){
  xRot_ = 25.0;
  yRot_ = -45.0;
}

void Scene::mousePressEvent(QMouseEvent *pe){
  mousePosPtr_ = pe->pos();
}

void Scene::mouseMoveEvent(QMouseEvent *pe){
  // вычисление углов поворота
  xRot_ += 180/nScale_ * static_cast<GLdouble>(pe->y()-mousePosPtr_.y()) / height();
  yRot_ += 180/nScale_ * static_cast<GLdouble>(pe->x()-mousePosPtr_.x()) / width();

  mousePosPtr_ = pe->pos();
  update();
}

void Scene::mouseReleaseEvent(QMouseEvent *pe){
  mousePosPtr_.~QPoint();
}

void Scene::keyPressEvent(QKeyEvent *event){
  switch (event->key()){
    case Qt::Key_Escape:
      this->close();
      break;
    case Qt::Key_R:
      resetRotation();
      break;
    case Qt::Key_Up:
      moveLightUp();
      break;
    case Qt::Key_Down:
      moveLightDown();
      break;
    case Qt::Key_Right:
      moveLightRight();
      break;
    case Qt::Key_Left:
      moveLightLeft();
      break;
    case Qt::Key_A:
      moveLightForth();
      break;
    case Qt::Key_D:
      moveLightBack();
      break;
    case Qt::Key_Space:
      if (!isAnmationRunning_){
        isAnmationRunning_ = GL_TRUE;
        timer = new QTimer(this);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(playAnimation()));
        timer->start(50);
      } else {
        isAnmationRunning_ = GL_FALSE;
        timer->stop();
        timer->~QTimer();
      }
      break;
  }
  update();
}

void Scene::moveLightUp(){
  lightPos_[1] += 0.05f;
}

void Scene::moveLightDown(){
  lightPos_[1] -= 0.05f;
}

void Scene::moveLightLeft(){
  lightPos_[0] -= 0.05f;
}

void Scene::moveLightRight(){
  lightPos_[0] += 0.05f;
}

void Scene::moveLightForth(){
  lightPos_[2] -= 0.05f;
}

void Scene::moveLightBack(){
  lightPos_[2] += 0.05f;
}

void Scene::playAnimation(){
  if (currFrame_ == frameCount_-1){
    isAnimAscanding_ = GL_FALSE;
  }

  if (currFrame_ == 0){
    isAnimAscanding_ = GL_TRUE;
  }

  if (isAnimAscanding_){
    currFrame_ += 1;
    curviness_ += curveStep_;
  } else {
    currFrame_ -= 1;
    curviness_ -= curveStep_;
  }

  update();
}

// ADDITIONAL FUNCTIONS //
// USUALLY FOR METH OBJECT RADIUS VATYING
GLdouble Scene::normDist(const GLdouble x, const GLdouble exp, const GLdouble dev){
  return 1. / (std::sqrt(M_PI * 2.) * dev) * std::exp(-(x - exp)*(x - exp) / (2. * dev * dev));
}

GLdouble Scene::radFuncPlynomial(const GLdouble arg, const GLdouble param){
  GLdouble buff = std::abs(2. * arg - 1.);
  return (3. - std::pow(buff, param) - std::pow(buff, param*2.) - std::pow(buff, param*3.)) / 3.;
}

GLdouble Scene::radFuncSine(const GLdouble arg, const GLdouble param){
  return .5 * std::sin(2.*M_PI * std::pow(arg, param) - M_PI/2.) + 0.55;
}

GLdouble Scene::radFuncPlum(const GLdouble arg, const GLdouble param){
  return std::sqrt(1. - (2.*arg-1.)*(2.*arg-1.)) * (1. - param * (arg - 1.));
}
