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
  BRepFP face = mesh->AddFace();
  face->AddLoop(loop);
  loop->SetFace(face);
}

BRepPP EulerOp::MEV(BRepMP mesh, BRepLP loop, BRepPP point, QVector3D pos)
{
  qDebug() << "MEV";
  if(mesh == nullptr || loop == nullptr || point == nullptr) return BRepPP(nullptr);
  BRepPP v2p = mesh->AddPoint();
  v2p->SetPosition(pos);
//  qDebug() << "0";
  BRepHEP fromp=nullptr, top=nullptr;
  for(int i=0; i<loop->HalfEdgeNum(); i++)
  {
//    qDebug() << "i=" << i;
    if(loop->HalfEdge(i)->From() == point)
    {
      fromp = loop->HalfEdge(i);
    }
  }
  if(loop->HalfEdgeNum()!=0 && (fromp==nullptr))
    return BRepPP(nullptr);
  if(fromp!=nullptr)
  {
    top=fromp->Prev();

//    qDebug() << "fromp"; fromp->Print();
//    qDebug() << "top"; top->Print();
  }

//  qDebug() << "1";
  BRepHEP he1p = mesh->AddHalfEdge();
  BRepHEP he2p = mesh->AddHalfEdge();
  he1p->SetFrom(point);
  he1p->SetTo(v2p);
  he1p->SetOpposite(he2p);
  he1p->SetNext(he2p);


  if(top!=nullptr)
    top->SetNext(he1p);
  he2p->SetFrom(v2p);
  he2p->SetTo(point);
  he2p->SetOpposite(he1p);
  he2p->SetPrev(he1p);
//  qDebug() << "2";

//  qDebug() << "he1p"; he1p->Print();
//  qDebug() << "he2p"; he2p->Print();

  if(fromp!=nullptr)
  {
    he2p->SetNext(fromp);
    fromp->SetPrev(he2p);
    he1p->SetPrev(top);
    top->SetNext(he1p);

//    qDebug() << "after";
//    qDebug() << "fromp"; fromp->Print();
//    qDebug() << "top"; top->Print();
//    qDebug() << "he1p"; he1p->Print();
//    qDebug() << "he2p"; he2p->Print();
  }
  if(loop->HalfEdgeNum()==0)
  {
    he1p->SetPrev(he2p);
    he2p->SetNext(he1p);
  }

//  qDebug() << "3";
  loop->AddHalfEdge(he1p);
  loop->AddHalfEdge(he2p);
  he1p->SetLoop(loop);
  he2p->SetLoop(loop);

//  qDebug() << "4";

  return v2p;
}

BRepFP EulerOp::MEF(BRepMP mesh, BRepLP &loop, BRepPP v1, BRepPP v2)
{
  qDebug() << "MEF";
  BRepHEP heS = loop->HalfEdge(0);
  BRepHEP heT = heS;
  while(heS->From()!=v1 && heS->From()!=v2)
  {
    qDebug() << "finding heS: " << heS << heS->From()->Position() << "->" << heS->To()->Position();
    heS=heS->Prev();
  }
  while(heT->To()!=v1 && heT->To()!=v2)
  {
    qDebug() << "finding heT: " << heT << heT->From()->Position() << "->" << heT->To()->Position();
    heT=heT->Next();
  }
  qDebug() << "heS: "; heS->Print();
  qDebug() << "heT: "; heT->Print();

  BRepHEP heS2 = heT->Next();
  BRepHEP heT2 = heS->Prev();
  qDebug() << "heS2: "; heS2->Print();
  qDebug() << "heT2: "; heT2->Print();
  qDebug() << "MEF 1";
  if(heS->From()!=heT->To())
  {
    BRepLP l2=mesh->AddLoop();
    for(BRepHEP he=heS; he->Prev()!=heT; he=he->Next())
    {
      qDebug() << "Remove from loop:" << he << he->From()->Position() << "->" << he->To()->Position() << "Prev()=" << he->Prev();
      loop->RemoveHalfEdge(he);
      l2->AddHalfEdge(he);
      he->SetLoop(l2);
    }
    qDebug() << "MEF 1";
    BRepHEP he1 = mesh->AddHalfEdge();
    BRepHEP he2 = mesh->AddHalfEdge();
    he1->SetFrom(heT->To());
    he1->SetTo(heS->From());
    he1->SetLoop(l2);
    he1->SetOpposite(he2);
    he1->SetNext(heS);
    he1->SetPrev(heT);
    heT->SetNext(he1);
    heS->SetPrev(he1);
    l2->AddHalfEdge(he1);
    qDebug() << "MEF 1";

    he2->SetFrom(he1->To());
    he2->SetTo(he1->From());
    he2->SetOpposite(he1);
    he2->SetLoop(loop);
    he2->SetNext(heS2);
    he2->SetPrev(heT2);
    heS2->SetPrev(he2);
    heT2->SetNext(he2);
    loop->AddHalfEdge(he2);
    qDebug() << "MEF 1";

    qDebug() << "l2:"; l2->Print();
    qDebug() << "loop:"; loop->Print();

    if(l2->HalfEdgeNum()>loop->HalfEdgeNum())
    {
      loop->Exchange(*l2);
    }
    qDebug() << "MEF 1";

    BRepFP nf = mesh->AddFace();
    nf->SetMesh(mesh);
    nf->AddLoop(l2);
    l2->SetFace(nf);
    qDebug() << "MEF 1";
    return nf;
  }
  else
  {
    qDebug() << "subloop";
    BRepLP temp = mesh->AddLoop();
    for(BRepHEP he=heS; he->Prev()!=heT; he=he->Next())
    {
      temp->AddHalfEdge(he);
      he->SetLoop(temp);
      loop->RemoveHalfEdge(he);
    }

    heT->Next()->SetPrev(heS->Prev());
    heS->Prev()->SetNext(heT->Next());
    heT->SetNext(heS);
    heS->SetPrev(heT);

    qDebug() << "temp:"; temp->Print();
    qDebug() << "Loop:"; loop->Print();

    BRepFP tempFace = MEF(mesh, loop, v1, v2);
    qDebug() << "tempFace:"; tempFace->Print();
    qDebug() << "loop:"; loop->Print();

    if(QVector3D::dotProduct(tempFace->Normal(), temp->Normal())<0)
    {
      qDebug() << "exchanged";
      loop->Exchange(*(tempFace->Loop(0)));
    }

    qDebug() << "tempFace:"; tempFace->Loop(0)->Print();
    qDebug() << "loop:"; loop->Print();

    BRepHEP heFrom = loop->HEFromV(heT->To());
    BRepHEP heTo = heFrom->Prev();

    qDebug() << "heFrom:"; heFrom->Print();
    qDebug() << "heTo:"; heTo->Print();

    for(int i=0; i<temp->HalfEdgeNum(); i++)
    {
      temp->HalfEdge(i)->SetLoop(loop);
      loop->AddHalfEdge(temp->HalfEdge(i));
    }
    heS->SetPrev(heTo);
    heTo->SetNext(heS);
    heT->SetNext(heFrom);
    heFrom->SetPrev(heT);
    temp->Clear();
    mesh->RemoveLoop(temp);

    qDebug() << "loop:"; loop->Print();
    qDebug() << "tempFace:"; tempFace->Print();

    return tempFace;
  }
}

BRepLP EulerOp::KEMR(BRepMP mesh, BRepLP loop, BRepHEP he)
{
  qDebug() << "KEMR";
  qDebug() << "1";
  BRepPP v1 = he->From();
  BRepPP v2 = he->To();

  qDebug() << "1";
  BRepHEP he2 = he->Opposite();

  BRepLP l2 = mesh->AddLoop();
  BRepHEP hei = he->Next();
  qDebug() << "1";
  while(hei!=he2)
  {
    qDebug() << "he:"; hei->Print();
    l2->AddHalfEdge(hei);
    hei=hei->Next();
  }
  he2->Prev()->SetNext(he->Next());
  he->Next()->SetPrev(he2->Prev());
  qDebug() << "l2:"; l2->Print();

  qDebug() << "1";
  hei = he2->Next();
  loop->Clear();
  while(hei!=he)
  {
    loop->AddHalfEdge(hei);
    hei=hei->Next();
  }
  qDebug() << "1";
  he->Prev()->SetNext(he2->Next());
  he2->Next()->SetPrev(he->Prev());
  qDebug() << "loop";
  loop->Print();

  qDebug() << "1";
  mesh->RemoveHalfEdge(he);
  mesh->RemoveHalfEdge(he2);

  qDebug() << "1";
  BRepFP r = loop->Face();
  r->AddLoop(l2);
  l2->SetFace(r);
  l2->SetDir(true);
  qDebug() << "1";

  return l2;
}

void EulerOp::KFMRH(BRepMP mesh, BRepFP f1, BRepFP f2)
{
  f1->AddLoop(f2->Loop(0));
  f2->Loop(0)->SetFace(f1);
  f2->Loop(0)->Reverse();
  f2->Loop(0)->SetDir(true);
  mesh->RemoveFace(f2);
}

void EulerOp::Sweep(BRepMP mesh, BRepFP f1, float d)
{
  qDebug() << "Sweep";

  BRepFP f2 = mesh->Clone(f1);
  qDebug() << "f2="; f2->Print();

  QVector3D n = -f1->Normal();
  QSet<BRepPP> set;
  for(int i=0; i<f1->LoopNum(); i++)
  {
    BRepLP loop = f1->Loop(i);
    for(int j=0; j<loop->HalfEdgeNum(); j++)
    {
      set.insert(loop->HalfEdge(j)->From());
    }
  }

  for(QSet<BRepPP>::iterator i=set.begin(); i!=set.end(); i++)
  {
    (*i)->SetPosition((*i)->Position()+d*n);
  }

  for(int i=0; i<f1->LoopNum(); i++)
  {
    BRepLP loop = f1->Loop(i);

    for(int j=0; j<loop->HalfEdgeNum(); j++)
    {
      BRepHEP he = loop->HalfEdge(j);
      BRepLP nlp = mesh->AddLoop();
      BRepHEP ophe = he->Opposite();

      BRepHEP he2op = mesh->AddHalfEdge();
      he2op->SetFrom(he->From());
      he2op->SetTo(ophe->To());
      he2op->SetLoop(nlp);
      nlp->AddHalfEdge(he2op);

      BRepHEP opophe = mesh->AddHalfEdge();
      opophe->SetFrom(ophe->To());
      opophe->SetTo(ophe->From());
      opophe->SetOpposite(ophe);
      opophe->SetPrev(he2op);
      opophe->SetLoop(nlp);
      he2op->SetNext(opophe);
      nlp->AddHalfEdge(opophe);

      BRepHEP op2he = mesh->AddHalfEdge();
      op2he->SetFrom(ophe->From());
      op2he->SetTo(he->To());
      op2he->SetLoop(nlp);
      op2he->SetPrev(opophe);
      opophe->SetNext(op2he);
      nlp->AddHalfEdge(op2he);

      BRepHEP heop = mesh->AddHalfEdge();
      heop->SetFrom(he->To());
      heop->SetTo(he->From());
      heop->SetNext(he2op);
      heop->SetPrev(op2he);
      heop->SetLoop(nlp);
      heop->SetOpposite(he);
      he2op->SetPrev(heop);
      op2he->SetNext(heop);
      nlp->AddHalfEdge(heop);

      he->SetOpposite(heop);
      ophe->SetOpposite(opophe);

      BRepFP nf = mesh->AddFace();
      nf->AddLoop(nlp);
      nlp->SetFace(nf);
    }

    for(int j=0; j<loop->HalfEdgeNum(); j++)
    {
      BRepHEP he=loop->HalfEdge(j);
      he->Opposite()->Next()->SetOpposite(he->Prev()->Opposite()->Prev());
      he->Prev()->Opposite()->Prev()->SetOpposite(he->Opposite()->Next());
    }
  }
}
