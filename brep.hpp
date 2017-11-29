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

template<class T> class _QLinkedList : public QLinkedList<T>
{
public:
  T& operator[](const int index)
  {
    int c=0;
    for(iterator i=begin(); c<index; i++, c++);
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

typedef QSP<BRepPoint>      BRepPP;
typedef QSP<BRepEdge>       BRepEP;
typedef QSP<BRepHalfEdge>   BRepHEP;
typedef QSP<BRepLoop>       BRepLP;
typedef QSP<BRepFace>       BRepFP;
typedef QSP<BRepMesh>       BRepMP;
typedef QSP<BRepObject>     BRepOP;
typedef QSP<BRepTexture>    BRepTP;
typedef QSP<BRepRenderData> BRepRP;

typedef _QLinkedList<BRepPoint>::iterator      PointIte;
typedef _QLinkedList<BRepEdge>::iterator       EdgeIte;
typedef _QLinkedList<BRepHalfEdge>::iterator   HEIte;
typedef _QLinkedList<BRepLoop>::iterator       LoopIte;
typedef _QLinkedList<BRepFace>::iterator       FaceIte;
typedef _QLinkedList<BRepMesh>::iterator       MeshIte;
typedef _QLinkedList<BRepTexture>::iterator    TextureIte;
typedef _QLinkedList<BRepRenderData>::iterator RDIte;

// class definiations
class BRepItem       : public QSharedData
{
public:
  bool valid;

public:
  BRepItem(bool _valid=false) : valid(_valid) {}

  BRepItem operator=(BRepItem b)
  {
    valid = b.valid;
  }
};

class BRepVisualItem : public BRepItem
{
public:
  bool visible;

public:
  BRepVisualItem(bool _valid=false, bool _visible=false) : BRepItem(_valid), visible(_visible) {}
};

class BRepPoint      : public BRepItem
{
private:
  _QLinkedList3D position;
  _QLinkedList3D normal;
  _QLinkedList<BRepHEP> fanOutList;

public:
  // get methods
  _QLinkedList3D Position() { return position; }
  _QLinkedList3D Normal() { return normal; }
  int FanOutNum() { return fanOutList.size(); }
  BRepHEP FanOutEdge(int index) { return fanOutList[index]; }

  // set methods
  void SetPosition(_QLinkedList3D pos) { position = pos; }
  void SetNormal(_QLinkedList3D n) { normal = n; }
  int AddFanOutEdge(BRepHEP edge) { fanOutList.push_back(edge); return fanOutList.size()-1;}
  bool RemoveFanOutEdge(BRepHEP index) { fanOutList.removeOne(index); }
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
};

class BRepLoop       : public BRepVisualItem
{
private:
  _QLinkedList<BRepHEP> hes;
  bool dir;

public:
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
    for(HEIte i = hes.begin(); i!=hes.end(); i++)
    {
      if(*i->From()==v)
      {
        return *i;
      }
    }
    return BRepHEP(nullptr);
  }

  BRepHEP HEToV(BRepPP v)
  {
    for(HEIte i = hes.begin(); i!=hes.end(); i++)
    {
      if(*i->To()==v)
      {
        return *i;
      }
    }
    return BRepHEP(nullptr);
  }
};

class BRepFace       : public BRepVisualItem
{
private:
  BRepTP texture;
//  _QLinkedList<BRepRenderData> renderData;
  _QLinkedList<BRepLP> loops;
  BRepMP mesh;
  _QLinkedList3D normal;

public:
  // get methods
  BRepTP Texture() { return texture; }
  int LoopNum() { return loops.size(); }
  BRepLP Loop(int index) { return loops[index]; }
//  int RenderDataNum() { return renderData.size(); }
//  BRepRP RenderData(int index) { return renderData[index]; }
  BRepMP Mesh() { return mesh; }
  _QLinkedList3D Normal() { return normal; }

  // set methods
  bool SetTexture(BRepTP t) { texture = t; }
  int AddLoop(BRepLP loop) { loops.push_back(loop); return loops.size()-1;}
  bool RemoveLoop(BRepLP index) { loops.removeOne(index); }
//  bool AddRenderData(BRepRenderData r) { renderData.push_back(r); }
//  bool RemoveRenderData(int index) { renderData.removeOne(index); }
  bool SetMesh(BRepMP m) { mesh = m; }
  void SetNormal(_QLinkedList3D n) { normal = n; }
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
  bool RemoveFace(BRepFP index) { faceLib.removeOne(index); }

  int AddPoint(BRepPoint p) { pointLib.push_back(p); return pointLib.size()-1; }
  BRepPP AddPoint()
  {
    BRepPoint p;
    pointLib.push_back(p);
    return BRepPP(&pointLib[pointLib.size()-1]);
  }
  bool RemovePoint(BRepPP index) { pointLib.removeOne(index); }

  int AddHalfEdge(BRepHalfEdge e) { halfEdgeLib.push_back(e); return halfEdgeLib.size()-1; }
  BRepHEP AddHalfEdge()
  {
    BRepHalfEdge he;
    halfEdgeLib.push_back(he);
    return BRepHEP(&halfEdgeLib[halfEdgeLib.size()-1]);
  }
  bool RemoveHalfEdge(BRepHEP index) { halfEdgeLib.removeOne(index); }

  int AddLoop(BRepLoop l) { loopLib.push_back(l); return loopLib.size()-1; }
  BRepLP AddLoop()
  {
    BRepLoop l;
    loopLib.push_back(l);
    return BRepLP(&loopLib[loopLib.size()-1]);
  }
  bool RemoveLoop(BRepLP index) { halfEdgeLib.removeOne(index); }

  void SetName(QString n) { name = n; }
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
  int AddMesh(BRepMesh m) { meshLib.push_back(m); return meshLib.size()-1; }
  bool RemoveMesh(BRepMP index) { meshLib.remove(index); }
//  bool AddTexture(BRepTexture text) { textureLib.push_back(text); }
  bool RemoveTexture(BRepTP index) { textureLib.remove(index); }
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
};

class BRepRenderData : public QSharedData
{
public:
  BRepHEP halfEdge;
  _QLinkedList3D color;
  _QLinkedList2D texcoord;
  bool dir;
};

#endif // BREP_HPP
