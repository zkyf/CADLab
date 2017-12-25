#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
  layout = new QHBoxLayout();

  displayer = new GLDisplayer();
  layout->addWidget(displayer);

  bl = new QVBoxLayout();

  button = new QPushButton("AddCube");
  button->setFixedWidth(50);
  bl->addWidget(button);

  sweepButton = new QPushButton("AddSweep");
  sweepButton->setFixedWidth(50);
  bl->addWidget(sweepButton);

  clear = new QPushButton("Clear");
  clear->setFixedWidth(50);
  bl->addWidget(clear);

  reset = new QPushButton("Reset");
  reset->setFixedWidth(50);
  bl->addWidget(reset);

  layout->addItem(bl);

  setLayout(layout);

  connect(button, &QPushButton::clicked, this, &MainWindow::AddCube);
  connect(sweepButton, &QPushButton::clicked, this, &MainWindow::AddSweepCube);
  connect(clear, &QPushButton::clicked, this, &MainWindow::Clear);
  connect(reset, &QPushButton::clicked, this, &MainWindow::Reset);

  show();
}

void MainWindow::AddCube()
{
  BRepOP object;
  object = new BRepObject;
  EulerOp::MVFS(object, QVector3D(-1, -1, -2));

  BRepMP mesh = object->Mesh(0);
  BRepLP loop = mesh->Loop(0);
  BRepPP v1 = mesh->Point(0);
  BRepPP v2 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, 1, -2));
  BRepPP v3 = EulerOp::MEV(mesh, loop, v2, QVector3D(1, 1, -2));
  BRepPP v4 = EulerOp::MEV(mesh, loop, v3, QVector3D(1, -1, -2));
  BRepFP f1234 = EulerOp::MEF(mesh, loop, v1, v4);

  BRepPP v5 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, -1, -4));
  BRepPP v6 = EulerOp::MEV(mesh, loop, v2, QVector3D(-1, 1, -4));
  BRepFP f1265 = EulerOp::MEF(mesh, loop, v5, v6);

  BRepPP v7 = EulerOp::MEV(mesh, loop, v3, QVector3D(1, 1, -4));
  BRepPP v8 = EulerOp::MEV(mesh, loop, v4, QVector3D(1, -1, -4));
  BRepFP f2376 = EulerOp::MEF(mesh, loop, v7, v6);

  BRepFP f3487 = EulerOp::MEF(mesh, loop, v8, v7);


  EulerOp::MEF(mesh, loop, v8, v5);
  BRepFP f5678 = loop->Face();

  BRepLP f1234lp = f1234->Loop(0);

  BRepPP v9 = EulerOp::MEV(mesh,  f1234lp, v1, QVector3D(-0.5, -0.5, -2));
  BRepPP v10 = EulerOp::MEV(mesh, f1234lp, v9, QVector3D(-0.5, 0.5, -2));
  BRepPP v11 = EulerOp::MEV(mesh, f1234lp, v10, QVector3D(0.5, 0.5, -2));
  BRepPP v12 = EulerOp::MEV(mesh, f1234lp, v11, QVector3D(0.5, -0.5, -2));

  BRepFP f9101112 = EulerOp::MEF(mesh, f1234lp, v12, v9);

  BRepHEP he19 = f1234lp->HEFromV1ToV2(v1, v9);

  BRepLP lp = EulerOp::KEMR(mesh, f1234lp, he19);
//  qDebug() << "after kemr f1234="; f1234lp->Face()->Print();
  qDebug() << "f1234 area=" << f1234->Area();

  BRepLP f9101112lp = f9101112->Loop(0);
  BRepPP v13 = EulerOp::MEV(mesh, f9101112lp, v9, QVector3D(-0.5, -0.5, -4));
  BRepPP v14 = EulerOp::MEV(mesh, f9101112lp, v10, QVector3D(-0.5, 0.5, -4));
  BRepFP f9101413 = EulerOp::MEF(mesh, f9101112lp, v13, v14);

  BRepPP v15 = EulerOp::MEV(mesh, f9101112lp, v11, QVector3D(0.5, 0.5, -4));
  BRepFP f10111514 = EulerOp::MEF(mesh, f9101112lp, v14, v15);

  BRepPP v16 = EulerOp::MEV(mesh, f9101112lp, v12, QVector3D(0.5, -0.5, -4));
  BRepFP f11121615 = EulerOp::MEF(mesh, f9101112lp, v16, v15);

  BRepFP f9121613 = EulerOp::MEF(mesh, f9101112lp, v13, v16);

  EulerOp::KFMRH(mesh, f5678, f9101112);

  displayer->SetBRepModel(object);
}

void MainWindow::AddSweepCube()
{
  BRepOP object;
  object = new BRepObject;
  EulerOp::MVFS(object, QVector3D(-1, -1, -2));

  BRepMP mesh = object->Mesh(0);
  BRepLP loop = mesh->Loop(0);
  BRepPP v1 = mesh->Point(0);
  BRepPP v2 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, 1, -2));
  BRepPP v3 = EulerOp::MEV(mesh, loop, v2, QVector3D( 1, 1, -2));
  BRepPP v4 = EulerOp::MEV(mesh, loop, v3, QVector3D( 1,-1, -2));

  BRepFP f1234 = EulerOp::MEF(mesh, loop, v1, v4);
  mesh->RemoveFace(f1234);

  BRepPP v5 = EulerOp::MEV(mesh, loop, v1, QVector3D(-0.9, -0.9, -2));
  BRepPP v6 = EulerOp::MEV(mesh, loop, v5, QVector3D(-0.9, 0.1, -2));
  BRepPP v7 = EulerOp::MEV(mesh, loop, v6, QVector3D(0.1, 0.1, -2));
  BRepPP v7_5 = EulerOp::MEV(mesh, loop, v7, QVector3D(0.3, -0.5, -2));
  BRepPP v8 = EulerOp::MEV(mesh, loop, v7_5, QVector3D(0.1, -0.9, -2));

  BRepFP f5678 = EulerOp::MEF(mesh, loop, v5, v8);
  mesh->RemoveFace(f5678);

  BRepHEP he15 = loop->HEFromV1ToV2(v1, v5);
  EulerOp::KEMR(mesh, loop, he15);

  BRepPP v9 = EulerOp::MEV(mesh, loop, v3,   QVector3D(0.9, 0.9, -2));
  BRepPP v10 = EulerOp::MEV(mesh, loop, v9,  QVector3D(0.9, -0.1, -2));
  BRepPP v11 = EulerOp::MEV(mesh, loop, v10, QVector3D(-0.1, -0.1, -2));
//  BRepPP v12 = EulerOp::MEV(mesh, loop, v11, QVector3D(0.2, 0.8, -2));
  BRepFP f9101112 = EulerOp::MEF(mesh, loop, v9, v11);
  mesh->RemoveFace(f9101112);
  BRepHEP he39 = loop->HEFromV1ToV2(v3, v9);
  EulerOp::KEMR(mesh, loop, he39);

  BRepFP f2 = EulerOp::Sweep(mesh, loop->Face(), 3.0, QVector3D(0, -0.5, -3));
//  BRepFP f3 = EulerOp::Sweep(mesh, f2, 3.0, QVector3D(0, 0.5, -3));
  mesh->RemoveFace(f2);
  displayer->SetBRepModel(object);
}

void MainWindow::Clear()
{
  displayer->SetBRepModel(nullptr);
  displayer->CameraSetRotation(0, 0, 0, 0);
  displayer->CameraSetTranslation(0, 0, 0);
}

void MainWindow::Reset()
{
  displayer->CameraSetRotation(0, 0, 0, 0);
  displayer->CameraSetTranslation(0, 0, 0);
}
