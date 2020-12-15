#include "scene.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);

  QApplication a(argc, argv);
  Scene w;
  w.show();
  return a.exec();
}
