#include "main_window.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);

  const QIcon icon{application.applicationDirPath() + "/resources/images/icon.png"};
  application.setWindowIcon(icon);

  MainWindow window(icon);
  window.show();

  return application.exec();
}
