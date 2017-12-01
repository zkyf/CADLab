#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
  layout = new QHBoxLayout();

  displayer = new GLDisplayer();
  layout->addWidget(displayer);

  button = new QPushButton("AddCube");
  button->setFixedWidth(50);
  layout->addWidget(button);

  setLayout(layout);

  connect(button, &QPushButton::clicked, this, &MainWindow::AddCube);

  show();
}

void MainWindow::AddCube()
{
  qDebug() << "AddCube";
  BRepOP object;
  object = new BRepObject;
  EulerOp::MVFS(object, QVector3D(-1, -1, -2));
  qDebug() << "MVFSed";
  BRepMP mesh = object->Mesh(0);
  BRepLP loop = mesh->Loop(0);
  BRepPP v1 = mesh->Point(0);
  BRepPP v2 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, 1, -2));
//  qDebug() << "MEVed";
  BRepPP v3 = EulerOp::MEV(mesh, loop, v2, QVector3D(1, 1, -2));
  BRepPP v4 = EulerOp::MEV(mesh, loop, v3, QVector3D(1, -1, -2));
  BRepFP f1234 = EulerOp::MEF(mesh, loop, v1, v4);
  qDebug() << "after f1234: ";
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "loop[" << i << "]=" << loop->HalfEdge(i)->From()->Position() << "->" << loop->HalfEdge(i)->To()->Position();
  }
  BRepPP v5 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, -1, -4));
  BRepPP v6 = EulerOp::MEV(mesh, loop, v2, QVector3D(-1, 1, -4));
  BRepFP f1265 = EulerOp::MEF(mesh, loop, v5, v6);
  qDebug() << "after f1265: ";
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "loop[" << i << "]=" << loop->HalfEdge(i)->From()->Position() << "->" << loop->HalfEdge(i)->To()->Position();
  }
  BRepPP v7 = EulerOp::MEV(mesh, loop, v3, QVector3D(1, 1, -4));
  BRepPP v8 = EulerOp::MEV(mesh, loop, v4, QVector3D(1, -1, -4));
  BRepFP f2376 = EulerOp::MEF(mesh, loop, v7, v6);
  qDebug() << "after f2376: ";
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "loop[" << i << "]=" << loop->HalfEdge(i)->From()->Position() << "->" << loop->HalfEdge(i)->To()->Position();
  }
  BRepFP f3487 = EulerOp::MEF(mesh, loop, v8, v7);
  qDebug() << "after f3487: ";
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "loop[" << i << "]=" << loop->HalfEdge(i)->From()->Position() << "->" << loop->HalfEdge(i)->To()->Position();
  }
  BRepFP f5678 = EulerOp::MEF(mesh, loop, v8, v5);
  qDebug() << "after f5678: ";
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "loop[" << i << "]=" << loop->HalfEdge(i)->From()->Position() << "->" << loop->HalfEdge(i)->To()->Position();
  }
  displayer->SetBRepModel(object);
}
