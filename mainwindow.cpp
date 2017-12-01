#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
  layout = new QHBoxLayout();

  displayer = new GLDisplayer();
  layout->addWidget(displayer);

  button = new QPushButton("AddCube");
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
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "loop[" << i << "]=" << loop->HalfEdge(i)->From()->Position() << "->" << loop->HalfEdge(i)->To()->Position();
  }
  BRepPP v5 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, -1, -4));
  BRepPP v6 = EulerOp::MEV(mesh, loop, v2, QVector3D(-1, 1, -4));
  BRepFP f1265 = EulerOp::MEF(mesh, loop, v5, v6);

  displayer->SetBRepModel(object);
}
