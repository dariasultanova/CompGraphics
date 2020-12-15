#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "src/SOIL.h"

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 0.0f;
int c = 8;
GLuint  texture;
int width, height;
unsigned char* image;

// Глобальные массивы свойств материала и освещения
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.5, 0.8, 1.0 };
GLfloat color[] = { 0.8, 0.5, 0.8, 0.5 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };// Цвет блика белый
GLfloat no_shininess[] = { 0.0 }; // Размер блика (обратная пропорция)
GLfloat high_shininess[] = { 128.0 }; // Размер блика (маленькая площадь - большой фокус)
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };// Цвет
GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };// Расположение источника
GLfloat light_spot_direction[] = { 0.0, 0.0, -1.0, 1.0 };
GLfloat const_att = 1.0;
GLfloat l_att = 0.7;
GLfloat q_att = 1.8;
GLfloat spot = 15;

void SetLightMultiMaterial() {
    glClearColor(0.3, 0.3, 0.3, 0.0);
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

    // Тор
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glutSolidTorus(0.2, 0.5, 50, 50);
    glPopMatrix();

    // Цилиндр
    glPushMatrix();
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glTranslatef(-2.1, 0.0, 0.0);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    GLUquadricObj* quadObj;
    quadObj = gluNewQuadric();
    gluCylinder(quadObj, 0.4, 0.4, 1.2, 50, 50);
    gluQuadricOrientation(quadObj, GLU_INSIDE);
    gluDisk(quadObj, 0.0, 0.4, 50, 50);
    glTranslatef(0.0, 0.0, 1.2);
    gluQuadricOrientation(quadObj, GLU_OUTSIDE);
    gluDisk(quadObj, 0.0, 0.4, 50, 50);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();

    // Октаэдр
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTranslatef(2.1, 0.0, 0.0);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glutSolidOctahedron();
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
        glOrtho(-1.5, 1.5, -1.5 / aspectRatio, 1.5 / aspectRatio, -10.0, 10.0);
    else
        glOrtho(-1.5 * aspectRatio, 1.5 * aspectRatio, -1.5, 1.5, -10, 10);

    // Восстановливает матрицу преобразования в исходный режим вида
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        xRot -= 5.0f;

    if (key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;

    if (key == GLUT_KEY_PAGE_UP)
        zRot -= 5.0f;

    if (key == GLUT_KEY_PAGE_DOWN)
        zRot += 5.0f;

    glutPostRedisplay();
}

void normalKeys(unsigned char key, int x, int y) {
    if (key == 'a') {
        light_position[0] = rand() % 3 - 1;
        light_position[1] = rand() % 3 - 1;
        light_position[2] = rand() % 3 - 1;
        while (light_position[0] == 0 && light_position[1] == 0 && light_position[2] == 0) {
            light_position[0] = rand() % 3 - 1;
            light_position[1] = rand() % 3 - 1;
            light_position[2] = rand() % 3 - 1;
        }
        std::cout << light_position[0] << " , " << light_position[1] << " , " << light_position[2] << "\n";
    }

    if (key == 's') {
        white_light[0] = (GLfloat)(rand() % 11) / 10.0;
        white_light[1] = (GLfloat)(rand() % 11) / 10.0;
        white_light[2] = (GLfloat)(rand() % 11) / 10.0;
        white_light[3] = (GLfloat)(rand() % 11) / 10.0;
        std::cout << white_light[0] << " , " << white_light[1] << " , " << white_light[2] << "\n";
    }

    if (key == 'd') {
        if (color[3] < 0.9)
            color[3] += 0.1;
        else
            color[3] = 0.5;
    }

    if (key == 'f') {

        if (c < 9)
            c++;
        else
            c = 0;

        if (c == 0) {
            l_att = 0.7;
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

    SetLightMultiMaterial();
    glutPostRedisplay();
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1400, 600);
    glutCreateWindow("cg2");
    image = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
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