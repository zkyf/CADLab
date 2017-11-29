#include "eulerop.h"

void EulerOp::MVFS(BRepOP *object, QVector3D pos)
{
  if(object == nullptr) return;
  *object = new BRepObject;
  BRepOP o = *object;
  BRepMesh mesh;
  BRepPoint point;
  BRepLoop loop;
  point.SetPosition(pos);
  mesh.AddPoint(point);
  mesh.AddLoop(loop);

  o->AddMesh(mesh);
}

BRepPP EulerOp::MEV(BRepMP mesh, BRepLP loop, BRepPP point, QVector3D pos)
{
  if(mesh.data() == nullptr || loop == nullptr || point == nullptr) return BRepPP(nullptr);
  BRepPoint v2;
  v2.SetPosition(pos);
  int index = mesh->AddPoint(v2);
  BRepPP v2p = mesh->Point(index);
  BRepHEP fromp, top;
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
    if(loop->HalfEdge(i)->From() == point)
    {
      fromp = loopHalfEdge(i);
    }
    if(loop->HalfEdge(i)->To() == point)
    {
      top = loopHalfEdge(i);
    }
  }
  if(loop->HalfEdgeNum()!=0 && (fromp.data()==nullptr || top.data()==nullptr))
    return BRepPP(nullptr);

  BRepHEP he1p = mesh->AddHalfEdge();
  BRepHEP he2p = mesh->AddHalfEdge();
  he1p->SetFrom(point);
  he1p->SetTo(v2);
  he1p->SetOpposite(he2p);
  he1p->SetNext(he2p);

  if(top.data()!=nullptr)
    top->SetNext(he1p);
  he2p->SetFrom(v2);
  he2p->SetTo(point);
  he2p->SetOpposite(he1p);
  if(fromp.data()!=nullptr)
    he2p->SetNext(fromp);
  if(loop->HalfEdgeNum()==0)
  {
    he1p->SetNext(he2p);
    he2p->SetNext(he1p);
  }

  loop->AddHalfEdge(he1p);
  loop->AddHalfEdge(he2p);

  return v2p;
}

BRepLP EulerOp::MEL(BRepMP mesh, BRepLP loop, BRepPP v1, BRepPP v2)
{
  BRepLP l2;

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
}
