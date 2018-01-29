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

BRepFP EulerOp::Sweep(BRepMP mesh, BRepFP f1, float d, QVector3D n)
{
  qDebug() << "Sweep";

  BRepFP f2 = mesh->Clone(f1);
  qDebug() << "f2="; f2->Print();

  n=n.normalized();
  QSet<BRepPP> set;
  for(int i=0; i<f2->LoopNum(); i++)
  {
    BRepLP loop = f2->Loop(i);
    for(int j=0; j<loop->HalfEdgeNum(); j++)
    {
      set.insert(loop->HalfEdge(j)->From());
    }
  }

  for(QSet<BRepPP>::iterator i=set.begin(); i!=set.end(); i++)
  {
    (*i)->SetPosition((*i)->Position()+d*n);
  }

  for(int i=0; i<f2->LoopNum(); i++)
  {
    BRepLP loop = f2->Loop(i);

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

  return f2;
}

void EulerOp::Subdivide(BRepOP object, BRepOP &out)
{
  if(object == nullptr || out == nullptr) return;

  int *aa = new int(100);
//  object->ext["test"] = ExtraInfo(aa, "int");
  object->ext.insert("test", ExtraInfo(aa, "int"));
  qDebug() << "inserted";
  qDebug() << "aa=" << *(int*)(object->ext["test"].p);

  const QString EdgePointName = "hep";
  const QString FacePointName = "fp";

  const QString FaceFaceName = "ff";
  const QString FaceLoopName = "fl";
  const QString FaceLastHEName = "flhets";

  const QString PointPointOriFace = "ppof";
  const QString PointFaceName = "pf";
  const QString PointLoopName = "pl";
  const QString PointLastHEName = "plhets";

  const QString EdgeEdgeName = "ehe";
  const QString EdgeStatusName = "estatus";

  for(int mid=0; mid<object->MeshNum(); mid++)
  {
    BRepMP mesh=object->Mesh(mid);
    BRepMP outMesh = out->AddMesh();

    for(int fid=0; fid<mesh->FaceNum(); fid++)
    {
      qDebug() << "";
      qDebug() << "fid=" << fid;
      BRepFP face = mesh->Face(fid);

      // generate points
      QVector3D* center = new QVector3D(0, 0, 0);
      int count=0;
      for(int lid=0; lid<face->LoopNum(); lid++)
      {
        qDebug() << "lid=" << lid;
        BRepLP loop = face->Loop(lid);
        count+=loop->HalfEdgeNum();
        for(int hid=0; hid<loop->HalfEdgeNum(); hid++)
        {
          qDebug() << "hid=" << hid;
          BRepHEP he = loop->HalfEdge(hid);
          QVector3D* hep = new QVector3D((he->From()->Position()+he->To()->Position())/2);
          qDebug() << "hep for he from " << he->From()->Position() << "->" << he->To()->Position() << "=" << *hep;
          he->ext[EdgePointName] = ExtraInfo(hep, "QVector3D");
          *center = (*center)+he->From()->Position();
          qDebug() << "center=" << *center;
        }
      }
      *center = (*center)/count;
      qDebug() << "*** center=" << *center;
      face->ext[FacePointName] = ExtraInfo(center, "QVector3D");
    }

    for(int fid=0; fid<mesh->FaceNum(); fid++)
    {
      BRepFP face = mesh->Face(fid);
      for(int lid=0; lid<face->LoopNum(); lid++)
      {
        BRepLP loop = face->Loop(lid);
        int hid=0;
        for(BRepHEP he=loop->HalfEdge(0); hid<loop->HalfEdgeNum(); hid++, he=he->Next())
        {
          BRepPP point = he->From();
          QVector3D np =(point->Position() + (*(QVector3D*)(he->ext[EdgePointName].p)) + (*(QVector3D*)(he->Prev()->ext[EdgePointName].p)) + (*(QVector3D*)(face->ext[FacePointName].p)))/4;
          qDebug() << "np for point " << point->Position() << "=" << np;
          BRepPP npp = outMesh->AddPoint();
          npp->ext[PointPointOriFace] = ExtraInfo(face, "BRepFP");
          npp->SetPosition(np);

          // create face face
          if(face->ext.find(FaceFaceName)==face->ext.end())
          {
            BRepFP ff = outMesh->AddFace();
            BRepLP fl = outMesh->AddLoop();
            BRepHEP flhets = outMesh->AddHalfEdge();
            ff->AddLoop(fl);
            ff->ext[FaceType] = ExtraInfo(nullptr, "Face");
            fl->AddHalfEdge(flhets);
            flhets->SetFrom(npp);
            flhets->SetTo(npp);
            flhets->SetPrev(nullptr);
            flhets->SetNext(nullptr);
            flhets->SetOpposite(nullptr);

            face->ext[FaceFaceName] = ExtraInfo(ff, "BRepFP", false);
            face->ext[FaceLoopName] = ExtraInfo(fl, "BRepLP", false);
            face->ext[FaceLastHEName] = ExtraInfo(flhets, "BRepHEP", false);
          }
          else
          {
            BRepLP fl = (BRepLP) face->ext[FaceLoopName].p;
            BRepHEP flhets = (BRepHEP) face->ext[FaceLastHEName].p;
            BRepHEP flhet = outMesh->AddHalfEdge();
            flhet->SetFrom(flhets->From());
            flhet->SetTo(npp);
            flhet->SetNext(flhets);
            flhets->SetFrom(npp);
            if(flhets->Prev()==nullptr)
            {
              flhet->SetPrev(flhets);
              flhets->SetNext(flhet);
            }
            else
            {
              flhet->SetPrev(flhets->Prev());
              flhets->Prev()->SetNext(flhet);
            }
            flhets->SetPrev(flhet);
            fl->AddHalfEdge(flhet);

            he->Prev()->ext[EdgeEdgeName] = ExtraInfo(flhet, "BRepHEP");
            if(hid==loop->HalfEdgeNum()-1)
            {
              he->ext[EdgeEdgeName] = ExtraInfo(flhets, "BRepHEP");
            }
          }

          // create vertex face
          if(point->ext.find(PointFaceName)==point->ext.end())
          {
            BRepFP npf = outMesh->AddFace();
            npf->ext[FaceType] = ExtraInfo(nullptr, "Point");
            BRepLP npl = outMesh->AddLoop();
            BRepHEP nphets = outMesh->AddHalfEdge();
            nphets->SetFrom(npp);
            nphets->SetTo(npp);
            nphets->SetPrev(nullptr);
            nphets->SetNext(nullptr);
            nphets->SetOpposite(nullptr);
            npf->AddLoop(npl);
            npl->AddHalfEdge(nphets);
            point->ext.insert(PointFaceName, ExtraInfo(npf, "BRepFP", false));
            point->ext.insert(PointLoopName, ExtraInfo(npl, "BRepLP", false));
            point->ext.insert(PointLastHEName, ExtraInfo(nphets, "BRepHEP", false));
          }
          else
          {
            BRepLP npl = (BRepLP) point->ext[PointLoopName].p;
            BRepHEP nphets = (BRepHEP) point->ext[PointLastHEName].p;
            BRepHEP nphet = outMesh->AddHalfEdge();
            nphet->SetFrom(nphets->From());
            nphet->SetTo(npp);
            nphet->SetNext(nphets);
            nphets->SetFrom(npp);
            if(nphets->Prev()==nullptr)
            {
              nphet->SetPrev(nphets);
              nphets->SetNext(nphet);
            }
            else
            {
              nphet->SetPrev(nphets->Prev());
              nphets->Prev()->SetNext(nphet);
            }
            nphets->SetPrev(nphet);
            npl->AddHalfEdge(nphet);
          }
        }
      }
    }

    // sort he in each vertex face
    qDebug() << endl;
    qDebug() << "sort he in each vertex face";
    for(int pid=0; pid<mesh->PointNum(); pid++)
    {
      qDebug() << endl;
      qDebug() << "pid=" << pid;
      BRepPP p = mesh->Point(pid);
      BRepLP pl = (BRepLP)p->ext[PointLoopName].p;
      if(pl->HalfEdgeNum()<4) continue;
      QVector<BRepPP> plist;
      for(int hid=0; hid<pl->HalfEdgeNum(); hid++)
      {
        plist.push_back(pl->HalfEdge(hid)->From());
      }
      for(int hid=0; hid<pl->HalfEdgeNum(); hid++)
      {
        outMesh->RemoveHalfEdge(pl->HalfEdge(hid));
      }
      pl->Clear();
      BRepFP cf = (BRepFP)plist[0]->ext[PointPointOriFace].p;
      BRepPP lp = plist[0];
      BRepHEP ts = outMesh->AddHalfEdge();
      ts->SetFrom(plist[0]);
      ts->SetTo(plist[0]);
      ts->SetPrev(ts);
      ts->SetNext(ts);
      pl->AddHalfEdge(ts);
      while(plist.size()>1)
      {
        qDebug() << "plist.size=" << plist.size();
        pl->Print();
        bool foundAdj = false;
        for(int i=1; i<plist.size(); i++)
        {
          BRepFP tp = (BRepFP)plist[i]->ext[PointPointOriFace].p;
          if(outMesh->Adjacent(cf, tp))
          {
            cf=tp;
            BRepHEP nhe = outMesh->AddHalfEdge();
            pl->AddHalfEdge(nhe);
            nhe->SetFrom(lp);
            nhe->SetTo(plist[i]);
            ts->SetFrom(plist[i]);
            nhe->SetNext(ts);
            nhe->SetPrev(ts->Prev());
            ts->Prev()->SetNext(nhe);
            ts->SetPrev(nhe);
            lp=plist[i];
            plist.remove(i);
            foundAdj=true;
            break;
          }
        }
        if(!foundAdj)
        {
          qDebug() << "*** Fatal error: no adjacant face found";
          break;
        }
      }

      qDebug() << "final pl";
      pl->Print();
    }

    // add edge faces
    for(int hid=0; hid<mesh->HalfEdgeNum(); hid++)
    {
      BRepHEP he0 = mesh->HalfEdge(hid);
      BRepHEP he0e = (BRepHEP)(he0->ext[EdgeEdgeName].p);
      BRepHEP he1 = he0->Opposite();
      BRepHEP he1e = (BRepHEP)(he1->ext[EdgeEdgeName].p);
      BRepPP p0 = he0->From();
      BRepPP p1 = he1->From();
      if(he0->ext.find(EdgeStatusName)!=he0->ext.end()) continue;
      he0->ext[EdgeStatusName] = ExtraInfo();
      he1->ext[EdgeStatusName] = ExtraInfo();

      BRepFP ef = outMesh->AddFace();
      ef->ext[FaceType] = ExtraInfo(nullptr, "Edge");
      BRepLP el = outMesh->AddLoop();
      ef->AddLoop(el);
      BRepHEP efhe0 = outMesh->AddHalfEdge();
      BRepHEP efhe1 = outMesh->AddHalfEdge();
      BRepHEP efhe2 = outMesh->AddHalfEdge();
      BRepHEP efhe3 = outMesh->AddHalfEdge();
      el->AddHalfEdge(efhe0);
      el->AddHalfEdge(efhe1);
      el->AddHalfEdge(efhe2);
      el->AddHalfEdge(efhe3);

      efhe0->SetFrom(he0e->To());
      efhe0->SetTo(he0e->From());
      efhe0->SetNext(efhe1);
      efhe0->SetPrev(efhe3);
      efhe0->SetOpposite(he0e);
      he0e->SetOpposite(efhe0);

      efhe1->SetFrom(he0e->From());
      efhe1->SetTo(he1e->To());
      efhe1->SetNext(efhe2);
      efhe1->SetPrev(efhe0);
      BRepLP p0l = (BRepLP)(p0->ext[PointLoopName].p);
      bool found = false;
      for(int hid=0; hid<p0l->HalfEdgeNum(); hid++)
      {
        BRepHEP p0lhe = p0l->HalfEdge(hid);
        if(p0lhe->From()==efhe1->To() && p0lhe->To()==efhe1->From())
        {
          p0lhe->SetOpposite(efhe1);
          efhe1->SetOpposite(p0lhe);
          found=true;
          break;
        }
      }
      if(!found)
      {
        p0l->Reverse();
        for(int hid=0; hid<p0l->HalfEdgeNum(); hid++)
        {
          BRepHEP p0lhe = p0l->HalfEdge(hid);
          if(p0lhe->From()==efhe1->To() && p0lhe->To()==efhe1->From())
          {
            p0lhe->SetOpposite(efhe1);
            efhe1->SetOpposite(p0lhe);
            found=true;
            break;
          }
        }
      }

      efhe2->SetFrom(he1e->To());
      efhe2->SetTo(he1e->From());
      efhe2->SetNext(efhe3);
      efhe2->SetPrev(efhe1);
      efhe2->SetOpposite(he1e);
      he1e->SetOpposite(efhe2);

      efhe3->SetFrom(he1e->From());
      efhe3->SetTo(he0e->To());
      efhe3->SetNext(efhe0);
      efhe3->SetPrev(efhe2);
      BRepLP p1l = (BRepLP)(p1->ext[PointLoopName].p);
      found = false;
      for(int hid=0; hid<p1l->HalfEdgeNum(); hid++)
      {
        BRepHEP p1lhe = p1l->HalfEdge(hid);
        if(p1lhe->From()==efhe3->To() && p1lhe->To()==efhe3->From())
        {
          p1lhe->SetOpposite(efhe3);
          efhe3->SetOpposite(p1lhe);
          found=true;
          break;
        }
      }
      if(!found)
      {
        p1l->Reverse();
        for(int hid=0; hid<p1l->HalfEdgeNum(); hid++)
        {
          BRepHEP p1lhe = p1l->HalfEdge(hid);
          if(p1lhe->From()==efhe3->To() && p1lhe->To()==efhe3->From())
          {
            p1lhe->SetOpposite(efhe3);
            efhe3->SetOpposite(p1lhe);
            found=true;
            break;
          }
        }
      }
    }
  }

  for(int mid=0; mid<out->MeshNum(); mid++)
  {
    BRepMP mesh = out->Mesh(mid);
    for(int fid=0; fid<mesh->FaceNum(); fid++)
    {
      mesh->SolveNonplaneFace(mesh->Face(fid));
    }
  }
}
