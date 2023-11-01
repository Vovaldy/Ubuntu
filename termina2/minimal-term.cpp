#include <QApplication>
#include <QKeySequence>
#include <QMainWindow>

#include "qtermwidget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow *mainWindow = new QMainWindow();

  QTermWidget *console = new QTermWidget();

  QFont font = QApplication::font();
  font.setFamily("Monospace");
  font.setPointSize(12);

  console->setTerminalFont(font);

  QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

  mainWindow->setCentralWidget(console);

  mainWindow->show();
  return app.exec();
}