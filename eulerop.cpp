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

}
