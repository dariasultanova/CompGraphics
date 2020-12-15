#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "src/SOIL.h"
#include "windows.h"
#include <dos.h>

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 0.0f;
int c = 8;
GLuint  texture;
int width, height;
unsigned char *image;
GLfloat x = 0.0;
GLfloat test_x;
GLfloat test2;
GLfloat test_y;
GLfloat test_z;
GLfloat rquad = 0;
int count = 0;

// Глобальные массивы свойств материала и освещения
GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_diffuse[] = { 0.8, 0.5, 0.8, 1.0 };
GLfloat color[] = { 0.8, 0.5, 0.8, 0.5 };
GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};// Цвет блика белый
GLfloat no_shininess[] = {0.0}; // Размер блика (обратная пропорция)
GLfloat high_shininess[] = {128.0}; // Размер блика (маленькая площадь - большой фокус)
                  // освещения, генерируемого источником
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };// Цвет
GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };// Расположение источника
GLfloat light_spot_direction[] = { 0.0, 0.0, -1.0, 1.0 };
GLfloat const_att = 1.0;
GLfloat l_att = 0.7;
GLfloat q_att = 1.8;
GLfloat spot = 15;

  
void SetLightMultiMaterial()
{
  glClearColor(0.3,0.3,0.3, 0.0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  
  // точечный источник света
  // убывание интенсивности с расстоянием задано функцией f(d) = 1.0 / ( const + KQ * d * d + KL * d)
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, const_att);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, l_att);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, q_att);
}

void RenderSceneMultiMaterial() {
  // Сбрасываем буфер цвета и буфер глубины
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  #define X .525731112119133606 
  #define Z .850650808352039932
  static GLfloat vdata[12][3] = {
    { -X, 0.0, Z },{ X, 0.0, Z },{ -X, 0.0, -Z },{ X, 0.0, -Z },
    { 0.0, Z, X },{ 0.0, Z, -X },{ 0.0, -Z, X },{ 0.0, -Z, -X },
    { Z, X, 0.0 },{ -Z, X, 0.0 },{ Z, -X, 0.0 },{ -Z, -X, 0.0 }
};

static GLuint tindices[20][3] = {
    { 0,4,1 },{ 0,9,4 },{ 9,5,4 },{ 4,5,8 },{ 4,8,1 },
    { 8,10,1 },{ 8,3,10 },{ 5,3,8 },{ 5,2,3 },{ 2,7,3 },
    { 7,10,3 },{ 7,6,10 },{ 7,11,6 },{ 11,0,6 },{ 0,1,6 },
    { 6,1,10 },{ 9,0,11 },{ 9,11,2 },{ 9,2,5 },{ 7,2,11 } };
int i;

  //Куб 1
glPushMatrix();
glEnable(GL_TEXTURE_2D);
glEnable(GL_TEXTURE_GEN_S);
glEnable(GL_TEXTURE_GEN_T);
glRotatef(xRot, 1.0f, 0.0f, 0.0f);
glRotatef(yRot, 0.0f, 1.0f, 0.0f);
glRotatef(zRot, 0.0f, 0.0f, 1.0f);
glRotatef(zRot, 0.0f, 0.0f, 1.0f);
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
glBegin(GL_QUADS);
           
  // Поверхность
  glVertex3d(1.5f, -0.65f, 1.0f);
  glVertex3d(-1.0f, -0.65f, 1.0f);
  glVertex3d(-1.0f, -0.65f, -1.0f);
  glVertex3d(1.5f, -0.65f, -1.0f);

  // Куб 2
  glTranslatef(0.0, -1.0, 0.0);
  glutSolidCube(1.3);
  glTranslatef(0.0, 1.0, 0.0);

  glEnd();

  glTranslatef(0.0, -0.25, 0);
  GLUquadricObj* quadObj;
  quadObj = gluNewQuadric();
  glTranslatef(test_x, test_y, test_z);
  glRotatef(-test2, 0, 0, 1);
  glutSolidCube(0.2);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}
  
void ChangeSizeMultiMaterial(int width, int height) {  
  // Индивидуальные настройки освещения
  SetLightMultiMaterial();
  
  // Устанавливает матрицу преобразования в режим проецирования
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Устанавливаем размеры ортогонального отсекающего объема
  GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;// Для соблюдения пропорций
  
  if (width <= height) 
        glOrtho (-1.5, 1.5, -1.5 / aspectRatio, 1.5 / aspectRatio, -10.0, 10.0);
    else 
        glOrtho (-1.5 * aspectRatio, 1.5 * aspectRatio, -1.5, 1.5, -10, 10);
  
  // Восстановливает матрицу преобразования в исходный режим вида
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
  
void SpecialKeys(int key, int x, int y) {
  if(key == GLUT_KEY_UP)
    xRot -= 5.0f;
  
  if(key == GLUT_KEY_DOWN)
    xRot += 5.0f;
  
  if(key == GLUT_KEY_LEFT)
    yRot -= 5.0f;
  
  if(key == GLUT_KEY_RIGHT)
    yRot += 5.0f;

  if (key == GLUT_KEY_PAGE_UP)
    zRot -= 5.0f;

  if (key == GLUT_KEY_PAGE_DOWN)
    zRot += 5.0f;
  
  glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y) {
    if (key == 'z') {
        light_position[0] = rand() % 3 - 1;
        light_position[1] = rand() % 3 - 1;
        light_position[2] = rand() % 3 - 1;

        while (light_position[0] == 0 && light_position[1] == 0 && light_position[2] ==0) {
            light_position[0] = rand() % 3 - 1;
            light_position[1] = rand() % 3 - 1;
            light_position[2] = rand() % 3 - 1;
        }
    std::cout << light_position[0] << " , " << light_position[1] << " , " << light_position[2] << "\n";
    }
    
    if (key == 'x') {
         white_light[0] = (GLfloat)(rand() % 11) / 10.0;
         white_light[1] = (GLfloat)(rand() % 11) / 10.0;
         white_light[2] = (GLfloat)(rand() % 11) / 10.0;
         white_light[3] = (GLfloat)(rand() % 11) / 10.0;
         std::cout << white_light[0] << " , " << white_light[1] << " , " << white_light[2] << "\n";
    }

    if (key == 'c') {
        if (c < 9)
            c++;
        else
            c = 0;

        if (c == 0) {
            l_att = 0.7 ;
            q_att = 1.8;
            light_position[3] = 1.0;
        }

        if (c == 1) {
            l_att = 0.35;
            q_att = 0.44;
        }

        if (c == 2) {
            l_att = 0.22;
            q_att = 0.20;
        }

        if (c == 3) {
            l_att = 0.14;
            q_att = 0.07;
        }

        if (c == 4) {
            l_att = 0.09;
            q_att = 0.032;
        }

        if (c == 5) {
            l_att = 0.07;
            q_att = 0.017;
        }

        if (c == 6) {
            l_att = 0.045;
            q_att = 0.0075;
        }

        if (c == 7) {
            l_att = 0.027;
            q_att = 0.0028;
        }

        if (c == 8) {
            l_att = 0.022;
            q_att = 0.0019;
        }

        if (c == 9) {
            light_position[3] = 0.0;
        }

        if (c == 10) {
            l_att = 0.0022;
            q_att = 0.000019;
        }

        std::cout << c << "\n";
    }

    if (key == 'd') {
        if (test_x < 0.8) {
            test_x += 0.1;

            test2 += 45;
            test_y = 0;
            if (count % 2 == 0) {
                test_y = 0.033;
            }
            count += 1;
        }
    }

    if (key == 'a') {
        if (test_x > -0.8) {
            test_x -= 0.1;

            test2 -= 45;
            test_y = 0;
            if (count % 2 == 0) {
                test_y = 0.033;
            }
            count += 1;
        }
    }

    if (key == 's') {
        if (test_y > -1.2) {
            test2 += 45;
            test_y -= 0.108;
        }
        
    }

    if (key == ' ') {
        test_x = 0;
        test2 = 0;
        test_y = 0;
        count = 0;
    }

 SetLightMultiMaterial();
 glutPostRedisplay();
}

void main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1200, 800);
  
  glutCreateWindow("course work");
  glClearColor(0.034, 0.521, 0.898, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  image = SOIL_load_image("t4.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glutReshapeFunc(ChangeSizeMultiMaterial);
  glutDisplayFunc(RenderSceneMultiMaterial);
  glutSpecialFunc(SpecialKeys);
  glutKeyboardFunc(normalKeys);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glutMainLoop();
}