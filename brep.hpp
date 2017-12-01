/*
 * Created on Oct 14 2017
 * This is done by Jiaxin Liu as part of the final project for
 * the CAD designing course in Zhejiang University
 */

#ifndef BREP_HPP
#define BREP_HPP

#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QLinkedList>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <utility>
using namespace std;

template<class T> class _QLinkedList : public QLinkedList<T>
{
public:
  T& operator[](int index)
  {
    int c=0;
    iterator i;
    for(i=begin(); c<index; i++, c++);
    return *i;
  }

  const T& operator[](int index) const
  {
    int c=0;
    const_iterator i;
    for(i=cbegin(); c<index; i++, c++);
    return *i;
  }
};

// list of all classes
class BRepItem;
class BRepVisualItem;
class BRepPoint;
class BRepEdge;
class BRepHalfEdge;
class BRepLoop;
class BRepFace;
class BRepMesh;
class BRepObject;
class BRepTexture;
class BRepRenderData;

#ifndef QSP
#define QSP QExplicitlySharedDataPointer
#endif

//typedef QSP<BRepPoint>      BRepPP;
//typedef QSP<BRepEdge>       BRepEP;
//typedef QSP<BRepHalfEdge>   BRepHEP;
//typedef QSP<BRepLoop>       BRepLP;
//typedef QSP<BRepFace>       BRepFP;
//typedef QSP<BRepMesh>       BRepMP;
//typedef QSP<BRepObject>     BRepOP;
//typedef QSP<BRepTexture>    BRepTP;
//typedef QSP<BRepRenderData> BRepRP;

typedef BRepPoint*      BRepPP;
typedef BRepEdge*       BRepEP;
typedef BRepHalfEdge*   BRepHEP;
typedef BRepLoop*       BRepLP;
typedef BRepFace*       BRepFP;
typedef BRepMesh*       BRepMP;
typedef BRepObject*     BRepOP;
typedef BRepTexture*    BRepTP;
typedef BRepRenderData* BRepRP;

typedef _QLinkedList<BRepPoint>::iterator      PointIte;
typedef _QLinkedList<BRepEdge>::iterator       EdgeIte;
typedef _QLinkedList<BRepHalfEdge>::iterator   HEIte;
typedef _QLinkedList<BRepLoop>::iterator       LoopIte;
typedef _QLinkedList<BRepFace>::iterator       FaceIte;
typedef _QLinkedList<BRepMesh>::iterator       MeshIte;
typedef _QLinkedList<BRepTexture>::iterator    TextureIte;
typedef _QLinkedList<BRepRenderData>::iterator RDIte;

typedef _QLinkedList<BRepPP>::iterator  PPIte;
typedef _QLinkedList<BRepEP>::iterator  EPIte;
typedef _QLinkedList<BRepHEP>::iterator HEPIte;
typedef _QLinkedList<BRepLP>::iterator  LPIte;
typedef _QLinkedList<BRepFP>::iterator  FPIte;
typedef _QLinkedList<BRepMP>::iterator  MPIte;
typedef _QLinkedList<BRepTP>::iterator  TPIte;
typedef _QLinkedList<BRepRP>::iterator  RDPIte;

// class definiations
class BRepItem       : public QSharedData
{
public:
  bool valid;

public:
  BRepItem(bool _valid=false) : valid(_valid) {}
  bool operator==(const BRepItem& b) { return true; }
};

class BRepVisualItem : public BRepItem
{
public:
  bool visible;

public:
  BRepVisualItem(bool _valid=false, bool _visible=false) : BRepItem(_valid), visible(_visible) {}
  bool operator==(const BRepVisualItem& b) { return true; }
};

class BRepPoint      : public BRepItem
{
private:
  QVector3D position;
  QVector3D normal;
  _QLinkedList<BRepHEP> fanOutList;

public:
  BRepPoint() {qDebug() << "c of BRepPoint @" << position << " with " << normal;}
  ~BRepPoint() { qDebug() << "dc of BRepPoint @" << position << " with " << normal; }

  // get methods
  QVector3D Position() { return position; }
  QVector3D Normal() { return normal; }
  int FanOutNum() { return fanOutList.size(); }
  BRepHEP FanOutEdge(int index) { return fanOutList[index]; }

  // set methods
  void SetPosition(QVector3D pos) { position = pos; }
  void SetNormal(QVector3D n) { normal = n; }
  int AddFanOutEdge(BRepHEP edge) { fanOutList.push_back(edge); return fanOutList.size()-1;}
  bool RemoveFanOutEdge(BRepHEP index) { fanOutList.removeOne(index); }

  bool operator==(const BRepPoint& b)
  {
    return (position == b.position) && (normal == b.normal);
  }
};

class BRepEdge       : public BRepItem
{
private:
  BRepMP mesh;
  BRepHEP he1, he2;

public:
  BRepMP Mesh() { return mesh; }
  void SetMesh(BRepMP m) { mesh=m; }
  BRepHEP He1() { return he1; }

  bool operator==(const BRepEdge& b)
  {
    return mesh==b.mesh && he1==b.he1 && he2==b.he2;
  }
};

class BRepHalfEdge   : public BRepItem
{
private:
  BRepPP from;
  BRepPP to;
  BRepHEP prev;
  BRepHEP next;
  BRepHEP opposite;
  BRepFP face;

public:
  // get methods
  BRepPP From() { return from; }
  BRepPP To() { return to; }
  BRepHEP Prev() { return prev; }
  BRepHEP Next() { return next; }
  BRepHEP Opposite() { return opposite; }
  BRepFP Face() { return face; }

  // set methods
  void SetFrom(BRepPP p) { from = p; }
  void SetTo(BRepPP p)  { to = p; }
  void SetPrev(BRepHEP e) { prev = e; }
  void SetNext(BRepHEP e) { next = e; }
  void SetOpposite(BRepHEP e) { opposite = e; }
  void SetFace(BRepFP f) { face = f; }

  bool operator==(const BRepHalfEdge& b)
  {
    return
        from==b.from &&
        to==b.to &&
        prev==b.prev &&
        next==b.next &&
        opposite==b.opposite &&
        face==b.face;
  }
};

class BRepLoop       : public BRepVisualItem
{
private:
  _QLinkedList<BRepHEP> hes;
  bool dir;

public:
  void operator=(BRepLoop& b)
  {
    dir = b.dir;
    hes = b.hes;
  }

  void Exchange(BRepLoop& b)
  {
    BRepLoop t = *this;
    *this = b;
    b = t;
  }

  int HalfEdgeNum() { return hes.size(); }
  BRepHEP HalfEdge(int index)
  {
    if(index>=0 && index<hes.size()) return hes[index];
    else return BRepHEP(nullptr);
  }
  int AddHalfEdge(BRepHEP he) { hes.push_back(he); return hes.size()-1;}
  bool RemoveHalfEdge(BRepHEP index)
  {
    hes.removeOne(index);
  }
  void Clear() { hes.clear(); }
  void SetDir(bool d) { dir = d; }
  bool Dir() { return dir; }

  bool VOnLoop(BRepPP v)
  {
    for(int i=0; i<hes.size(); i++)
    {
      if(hes[i]->From()==v)
        return true;
    }
    return false;
  }
  bool HEOnLoop(BRepHEP he)
  {
    for(int i=0; i<hes.size(); i++)
    {
      if(hes[i]==he)
        return true;
    }
    return false;
  }

  BRepHEP HEFromV(BRepPP v)
  {
    for(HEPIte i = hes.begin(); i!=hes.end(); i++)
    {
      if((*i)->From()==v)
      {
        return *i;
      }
    }
    return BRepHEP(nullptr);
  }

  BRepHEP HEToV(BRepPP v)
  {
    for(HEPIte i = hes.begin(); i!=hes.end(); i++)
    {
      if((*i)->To()==v)
      {
        return *i;
      }
    }
    return BRepHEP(nullptr);
  }

  bool operator==(const BRepLoop b)
  {
    if(hes.size()!=b.hes.size()) return false;
    for(int i=0; i<hes.size(); i++)
    {
      if(hes[i]!=b.hes[i])
      {
        return false;
      }
    }
    return true;
  }
};

class BRepFace       : public BRepVisualItem
{
private:
  BRepTP texture;
//  _QLinkedList<BRepRenderData> renderData;
  _QLinkedList<BRepLP> loops;
  BRepMP mesh;

public:
  // get methods
  bool IsPlane()
  {
    QVector3D normal = Normal();
    for(int i=0; i<loops.size(); i++)
    {
      BRepLP loop = loops[i];
      for(int j=0; j<loop->HalfEdgeNum(); j++)
      {
        QVector3D v1 = loop->HalfEdge(j)->From()->Position();
        QVector3D v2 = loop->HalfEdge(j)->To()->Position();
        QVector3D v3 = loop->HalfEdge(j)->Next()->To()->Position();

        QVector3D ln = QVector3D::crossProduct(v3-v2, v1-v2).normalized();
        if((ln-normal).length()>=1e-6)
        {
          return false;
        }
      }
    }
    return true;
  }

  BRepTP Texture() { return texture; }
  int LoopNum() { return loops.size(); }
  BRepLP Loop(int index) { return loops[index]; }
//  int RenderDataNum() { return renderData.size(); }
//  BRepRP RenderData(int index) { return renderData[index]; }
  BRepMP Mesh() { return mesh; }
  QVector3D Normal()
  {
//    return normal;
    if(loops.size()==0) return QVector3D(0, 0, 0);
    if(loops[0]->HalfEdgeNum()<3) return QVector3D(0, 0, 0);
    QVector3D v1 = loops[0]->HalfEdge(0)->From()->Position();
    QVector3D v2 = loops[0]->HalfEdge(0)->To()->Position();
    QVector3D v3 = loops[0]->HalfEdge(0)->Next()->To()->Position();
//    qDebug() << "Normal: " << v1 << v2 << v3;
    return QVector3D::crossProduct(v3-v2, v1-v2).normalized();
  }

  // set methods
  bool SetTexture(BRepTP t) { texture = t; }
  int AddLoop(BRepLP loop) { loops.push_back(loop); return loops.size()-1;}
  bool RemoveLoop(BRepLP index) { loops.removeOne(index); }
//  bool AddRenderData(BRepRenderData r) { renderData.push_back(r); }
//  bool RemoveRenderData(int index) { renderData.removeOne(index); }
  bool SetMesh(BRepMP m) { mesh = m; }
//  void SetNormal(QVector3D n) { normal = n; }

  bool operator==(const BRepFace& b)
  {
    if(texture!=b.texture) return false;
    if(mesh!=b.mesh) return false;
    if(loops!=b.loops) return false;
    return true;
  }
};

class BRepMesh       : public BRepVisualItem
{
private:
  _QLinkedList<BRepFace> faceLib;
  _QLinkedList<BRepPoint> pointLib;
  _QLinkedList<BRepHalfEdge> halfEdgeLib;
  _QLinkedList<BRepLoop> loopLib;
  BRepOP object;
  QString name;

public:
  BRepMesh() {qDebug() << "c of BRepMesh @";}
  ~BRepMesh() { qDebug() << "dc of BRepMesh @"; }

  // get methods
  int FaceNum() { return faceLib.size(); }
  BRepFP Face(int index) { return BRepFP(&faceLib[index]); }
  int PointNum() { return pointLib.size(); }
  BRepPP Point(int index) { return BRepPP(&pointLib[index]); }
  int HalfEdgeNum() { return halfEdgeLib.size(); }
  BRepHEP HalfEdge(int index) { return BRepHEP(&halfEdgeLib[index]); }
  int LoopNum() { return loopLib.size(); }
  BRepLP Loop(int index) { return BRepLP(&loopLib[index]); }
  BRepOP Object() { return object; }
  QString Name() { return name; }

  // set methods
  int AddFace(BRepFace f) { faceLib.push_back(f); return faceLib.size()-1; }
  BRepFP AddFace()
  {
    BRepFace face;
    faceLib.push_back(face);
    return BRepFP(&faceLib[faceLib.size()-1]);
  }
  void RemoveFace(BRepFP index) { faceLib.removeOne(*index); }

  int AddPoint(BRepPoint p) { pointLib.push_back(p); return pointLib.size()-1; }
  BRepPP AddPoint()
  {
    pointLib.push_back(BRepPoint());
    return BRepPP(&pointLib[pointLib.size()-1]);
  }
  void RemovePoint(BRepPP index) { pointLib.removeOne(*index); }

  int AddHalfEdge(BRepHalfEdge e) { halfEdgeLib.push_back(e); return halfEdgeLib.size()-1; }
  BRepHEP AddHalfEdge()
  {
    BRepHalfEdge he;
    halfEdgeLib.push_back(he);
    return BRepHEP(&halfEdgeLib[halfEdgeLib.size()-1]);
  }
  void RemoveHalfEdge(BRepHEP index) { halfEdgeLib.removeOne(*index); }

  int AddLoop(BRepLoop l) { loopLib.push_back(l); return loopLib.size()-1; }
  BRepLP AddLoop()
  {
    BRepLoop l;
    loopLib.push_back(l);
    return BRepLP(&loopLib[loopLib.size()-1]);
  }
  void RemoveLoop(BRepLP index) { loopLib.removeOne(*index); }

  void SetName(QString n) { name = n; }

  bool operator==(const BRepMesh& b)
  {
    if(faceLib!=b.faceLib) return false;
    if(pointLib!=b.pointLib) return false;
    if(halfEdgeLib!=b.halfEdgeLib) return false;
    if(loopLib!=b.loopLib) return false;
    if(object!=b.object) return false;
    return true;
  }
};

class BRepObject     : public BRepVisualItem
{
private:
  QString name;
  QString folder;
  _QLinkedList<BRepMesh> meshLib;
  _QLinkedList<BRepTexture> textureLib;

public:
  // get methods
  QString Name() { return name; }
  QString Folder() { return folder; }
  int MeshNum() { return meshLib.size(); }
  BRepMP Mesh(int index) { return BRepMP(&meshLib[index]); }
  int TextureNum() { return textureLib.size(); }
  BRepTP Texture(int index) { return BRepTP(&textureLib[index]); }

  // set methods
  BRepMP AddMesh() { BRepMesh mesh; meshLib.push_back(mesh); return BRepMP(&meshLib[meshLib.size()-1]); }
  int AddMesh(BRepMesh m) { meshLib.push_back(m); return meshLib.size()-1; }
  void RemoveMesh(BRepMP index) { meshLib.removeOne(*index); }
//  bool AddTexture(BRepTexture text) { textureLib.push_back(text); }
  void RemoveTexture(BRepTP index) { textureLib.removeOne(*index); }

  bool operator==(const BRepObject& b)
  {
    if(name!=b.name ||
       folder!=b.folder ||
       meshLib!=b.meshLib ||
       textureLib!=b.textureLib)
      return false;
    else
      return true;
  }
};

class BRepTexture    : public BRepItem
{
public:
  enum Type
  {
    None = -1,
    Diffuse = 0,
    Specular = 1,
    Normal = 2
  };

  QString filePath;
  Type type;

  bool operator==(const BRepTexture& b)
  {
    if(filePath!=b.filePath ||
       type!=b.filePath)
      return false;
    else
      return true;
  }
};

class BRepRenderData : public QSharedData
{
public:
  BRepHEP halfEdge;
  QVector3D color;
  QVector2D texcoord;
  bool dir;

  bool operator==(const BRepRenderData& b)
  {
    if(halfEdge!=b.halfEdge ||
       color!=b.color ||
       texcoord!=b.texcoord ||
       dir!=b.dir)
    {
      return false;
    }
    else
      return true;
  }
};

#endif // BREP_HPP
