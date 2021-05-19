#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  if (argc != 2 || argv == nullptr || *(argv + 1) == nullptr) return -1;
  std::string filename(*(argv + 1));

  QApplication a(argc, argv);
  MainWindow w;
  w.setFilename(filename);
  w.show();

  return a.exec();
}
