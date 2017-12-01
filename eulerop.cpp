#include "eulerop.h"

void EulerOp::MVFS(BRepOP &object, QVector3D pos)
{
  BRepMP mesh = object->AddMesh();
  qDebug() << "mesh add: " << mesh;
  BRepPP p = mesh->AddPoint();
  qDebug() << "point add: " << p;
  p->SetPosition(pos);
  qDebug() << p->Position();
  BRepLP loop = mesh->AddLoop();
  qDebug() << "loop add: " << loop;
}

BRepPP EulerOp::MEV(BRepMP mesh, BRepLP loop, BRepPP point, QVector3D pos)
{
  qDebug() << "MEV";
  if(mesh == nullptr || loop == nullptr || point == nullptr) return BRepPP(nullptr);
  BRepPP v2p = mesh->AddPoint();
  v2p->SetPosition(pos);
  qDebug() << "0";
  BRepHEP fromp, top;
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    qDebug() << "i=" << i;
    if(loop->HalfEdge(i)->From() == point)
    {
      fromp = loop->HalfEdge(i);
    }
    if(loop->HalfEdge(i)->To() == point)
    {
      top = loop->HalfEdge(i);
    }
  }
  if(loop->HalfEdgeNum()!=0 && (fromp==nullptr || top==nullptr))
    return BRepPP(nullptr);

  qDebug() << "1";
  BRepHEP he1p = mesh->AddHalfEdge();
  BRepHEP he2p = mesh->AddHalfEdge();
  he1p->SetFrom(point);
  he1p->SetTo(v2p);
  he1p->SetOpposite(he2p);
  he1p->SetNext(he2p);

  qDebug() << "2";
  if(top!=nullptr)
    top->SetNext(he1p);
  he2p->SetFrom(v2p);
  he2p->SetTo(point);
  he2p->SetOpposite(he1p);
  if(fromp!=nullptr)
    he2p->SetNext(fromp);
  if(loop->HalfEdgeNum()==0)
  {
    he1p->SetNext(he2p);
    he2p->SetNext(he1p);
  }

  qDebug() << "3";
  loop->AddHalfEdge(he1p);
  loop->AddHalfEdge(he2p);

  qDebug() << "4";

  return v2p;
}

BRepFP EulerOp::MEF(BRepMP mesh, BRepLP &loop, BRepPP v1, BRepPP v2)
{
  BRepLP l2 = mesh->AddLoop();

  BRepHEP heFromv1 = loop->HEFromV(v1);
  BRepHEP heTov1 = loop->HEToV(v1);

  BRepHEP heFromv2 = loop->HEFromV(v2);
  BRepHEP heTov2 = loop->HEToV(v2);

  BRepHEP hev1v2 = mesh->AddHalfEdge();
  BRepHEP hev2v1 = mesh->AddHalfEdge();

  heTov1->SetNext(hev1v2);
  hev1v2->SetFrom(v1);
  hev1v2->SetTo(v2);
  hev1v2->SetOpposite(hev2v1);
  hev1v2->SetNext(heFromv2);

  heTov2->SetNext(hev2v1);
  hev2v1->SetFrom(v2);
  hev2v1->SetTo(v1);
  hev2v1->SetOpposite(hev1v2);
  hev2v1->SetNext(heFromv1);

  loop->Clear();

  BRepHEP he = heFromv1;
  do
  {
    loop->AddHalfEdge(he);
    he = he->Next();
  }
  while(he!=heFromv1);

  he = heFromv2;
  do
  {
    l2->AddHalfEdge(he);
    he = he->Next();
  }
  while(he!=heFromv2);

  BRepFP face = mesh->AddFace();
  if(loop->HalfEdgeNum()<l2->HalfEdgeNum())
  {
    BRepLP t = l2;
    l2 = loop;
    loop = t;
  }
  face->AddLoop(l2);

  return face;
}

//BRepLP EulerOp::KEMR(BRepMP mesh, BRepLP loop, BRepHEP he)
//{
//  BRepPP v1 = he->From();
//  BRepPP v2 = he->To();

//  BRepHEP he2 = he->Opposite();

//  BRepLP l2 = mesh->AddLoop();
//  BRepHEP hei = he->Next();
//  while(hei->Next()!=he2)
//  {
//    l2->AddHalfEdge(hei);
//  }

//  hei = he2->Next();
//  loop->Clear();
//  while(hei->Next()!=he)
//  {
//    loop->AddHalfEdge(hei);
//  }

//  he->Prev()->SetNext(he2->Next());
//  he->Next()->SetPrev(he2->Prev());
//  he2->Prev()->SetNext(he->Next());
//  he2->Next()->SetPrev(he->Prev());

//  mesh->RemoveHalfEdge(he);
//  mesh->RemoveHalfEdge(he2);

//  return l2;
//}

//void EulerOp::KFMRH(BRepMP mesh, BRepFP f1, BRepFP f2)
//{
//}
