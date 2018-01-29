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
  void AddHoleCube();
  void AddSweepCube();
  void Clear();
  void Reset();
  void Subdivide();

private:
  QLayout* layout;
  GLDisplayer* displayer;

  QLayout* bl;
  QPushButton* cube;
  QPushButton* holecube;
  QPushButton* sweepButton;
  QPushButton* clear;
  QPushButton* reset;
  QPushButton* subd;

  BRepOP current;
};

#endif // MAINWINDOW_H
