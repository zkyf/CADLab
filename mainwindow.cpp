#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  layout = new QHBoxLayout();

  displayer = new GLDisplayer();
  layout->addWidget(displayer);

  setLayout(layout);
}
