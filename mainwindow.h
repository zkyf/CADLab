#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ModelDisplayGL.h"
#include "brep.hpp"
#include "eulerop.h"

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:
  void AddCube();
  void AddSweepCube();
  void Clear();
  void Reset();

private:
  QLayout* layout;
  GLDisplayer* displayer;

  QLayout* bl;
  QPushButton* button;
  QPushButton* sweepButton;
  QPushButton* clear;
  QPushButton* reset;
};

#endif // MAINWINDOW_H
