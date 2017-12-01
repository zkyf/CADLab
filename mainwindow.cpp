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
//  for(int fid=0; fid<mesh->FaceNum(); fid++)
//  {
//    BRepFP face = mesh->Face(fid);
//    qDebug() << "==================";
//    qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//    qDebug() << "normal = " << face->Normal();
//    qDebug() << "face half edges: ";
//    for(int i=0; i<face->LoopNum(); i++)
//    {
//      qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face();
//      BRepLP l = face->Loop(i);
//      for(int j=0; j<l->HalfEdgeNum(); j++)
//      {
//        BRepHEP he = l->HalfEdge(j);
//        qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//      }
//    }
//  }

  BRepFP f1234 = EulerOp::MEF(mesh, loop, v1, v4);
  qDebug() << "after f1234: ";

//  for(int fid=0; fid<mesh->FaceNum(); fid++)
//  {
//    BRepFP face = mesh->Face(fid);
//    qDebug() << "==================";
//    qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//    qDebug() << "normal = " << face->Normal();
//    qDebug() << "face half edges: ";
//    for(int i=0; i<face->LoopNum(); i++)
//    {
//      qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face();
//      BRepLP l = face->Loop(i);
//      for(int j=0; j<l->HalfEdgeNum(); j++)
//      {
//        BRepHEP he = l->HalfEdge(j);
//        qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//      }
//    }
//  }

  BRepPP v5 = EulerOp::MEV(mesh, loop, v1, QVector3D(-1, -1, -4));
  BRepPP v6 = EulerOp::MEV(mesh, loop, v2, QVector3D(-1, 1, -4));
  BRepFP f1265 = EulerOp::MEF(mesh, loop, v5, v6);
  qDebug() << "after f1265: ";

//  for(int fid=0; fid<mesh->FaceNum(); fid++)
//  {
//    BRepFP face = mesh->Face(fid);
//    qDebug() << "==================";
//    qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//    qDebug() << "normal = " << face->Normal();
//    qDebug() << "face half edges: ";
//    for(int i=0; i<face->LoopNum(); i++)
//    {
//      qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face();
//      BRepLP l = face->Loop(i);
//      for(int j=0; j<l->HalfEdgeNum(); j++)
//      {
//        BRepHEP he = l->HalfEdge(j);
//        qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//      }
//    }
//  }

  BRepPP v7 = EulerOp::MEV(mesh, loop, v3, QVector3D(1, 1, -4));
  BRepPP v8 = EulerOp::MEV(mesh, loop, v4, QVector3D(1, -1, -4));
  BRepFP f2376 = EulerOp::MEF(mesh, loop, v7, v6);
  qDebug() << "after f2376: ";

//  for(int fid=0; fid<mesh->FaceNum(); fid++)
//  {
//    BRepFP face = mesh->Face(fid);
//    qDebug() << "==================";
//    qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//    qDebug() << "normal = " << face->Normal();
//    qDebug() << "face half edges: ";
//    for(int i=0; i<face->LoopNum(); i++)
//    {
//      qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face();
//      BRepLP l = face->Loop(i);
//      for(int j=0; j<l->HalfEdgeNum(); j++)
//      {
//        BRepHEP he = l->HalfEdge(j);
//        qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//      }
//    }
//  }

  BRepFP f3487 = EulerOp::MEF(mesh, loop, v8, v7);
  qDebug() << "after f3487: ";

//  for(int fid=0; fid<mesh->FaceNum(); fid++)
//  {
//    BRepFP face = mesh->Face(fid);
//    qDebug() << "==================";
//    qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//    qDebug() << "normal = " << face->Normal();
//    qDebug() << "face half edges: ";
//    for(int i=0; i<face->LoopNum(); i++)
//    {
//      qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face();
//      BRepLP l = face->Loop(i);
//      for(int j=0; j<l->HalfEdgeNum(); j++)
//      {
//        BRepHEP he = l->HalfEdge(j);
//        qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//      }
//    }
//  }

  BRepFP f5678 = EulerOp::MEF(mesh, loop, v8, v5);
  qDebug() << "after f5678: ";

//  for(int fid=0; fid<mesh->FaceNum(); fid++)
//  {
//    BRepFP face = mesh->Face(fid);
//    qDebug() << "==================";
//    qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//    qDebug() << "normal = " << face->Normal();
//    qDebug() << "face half edges: ";
//    for(int i=0; i<face->LoopNum(); i++)
//    {
//      qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face();
//      BRepLP l = face->Loop(i);
//      for(int j=0; j<l->HalfEdgeNum(); j++)
//      {
//        BRepHEP he = l->HalfEdge(j);
//        qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//      }
//    }
//  }

  BRepLP f1234lp = f1234->Loop(0);
  qDebug() << "original:";
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  BRepPP v9 = EulerOp::MEV(mesh,  f1234lp, v1, QVector3D(-0.5, -0.5, -2));
  qDebug() << "after mev v9:";
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  BRepPP v10 = EulerOp::MEV(mesh, f1234lp, v9, QVector3D(-0.5, 0.5, -2));
  qDebug() << "after mev v10:";
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  BRepPP v11 = EulerOp::MEV(mesh, f1234lp, v10, QVector3D(0.5, 0.5, -2));
  qDebug() << "after mev v11:";
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  BRepPP v12 = EulerOp::MEV(mesh, f1234lp, v11, QVector3D(0.5, -0.5, -2));
  qDebug() << "after mev v12:";
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  BRepFP f9101112 = EulerOp::MEF(mesh, f1234lp, v12, v9);
  qDebug() << "after MEF f9101112:";
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  qDebug() << "loop f9101112:";
  for(int i=0; i<f9101112->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f9101112->Loop(i)->Face();
    BRepLP l = f9101112->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }
  BRepHEP he19 = NULL;
  for(int i=0; i<f1234lp->HalfEdgeNum(); i++)
  {
    if(f1234lp->HalfEdge(i)->From()==v1 && f1234lp->HalfEdge(i)->To()==v9)
    {
      he19=f1234lp->HalfEdge(i);
      break;
    }
  }
  qDebug() << "he19=" << he19 << he19->From()->Position() << "->" << he19->To()->Position();
  BRepLP lp = EulerOp::KEMR(mesh, f1234lp, f9101112, he19);
  for(int i=0; i<f1234->LoopNum(); i++)
  {
    qDebug() << "  loop#" << i << " face add=" << f1234->Loop(i)->Face();
    BRepLP l = f1234->Loop(i);
    for(int j=0; j<l->HalfEdgeNum(); j++)
    {
      BRepHEP he = l->HalfEdge(j);
      qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
    }
  }

  displayer->SetBRepModel(object);
}
