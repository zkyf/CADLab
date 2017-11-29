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
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

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

typedef QVector<BRepPoint>::iterator      PointIte;
typedef QVector<BRepEdge>::iterator       EdgeIte;
typedef QVector<BRepHalfEdge>::iterator   HEIte;
typedef QVector<BRepLoop>::iterator       LoopIte;
typedef QVector<BRepFace>::iterator       FaceIte;
typedef QVector<BRepMesh>::iterator       MeshIte;
typedef QVector<BRepTexture>::iterator    TextureIte;
typedef QVector<BRepRenderData>::iterator RDIte;

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
  QVector3D position;
  QVector3D normal;
  QVector<BRepHEP> fanOutList;

public:
  // get methods
  QVector3D Position() { return position; }
  QVector3D Normal() { return normal; }
  int FanOutNum() { return fanOutList.size(); }
  BRepHEP FanOutEdge(int index) { return fanOutList[index]; }

  // set methods
  void SetPosition(QVector3D pos) { position = pos; }
  void SetNormal(QVector3D n) { normal = n; }
  int AddFanOutEdge(BRepHEP edge) { fanOutList.push_back(edge); return fanOutList.size()-1;}
  bool RemoveFanOutEdge(int index) { fanOutList.remove(index); }
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
  BRepHEP next;
  BRepHEP opposite;
  BRepFP face;

public:
  // get methods
  BRepPP From() { return from; }
  BRepPP To() { return to; }
  BRepHEP Next() { return next; }
  BRepHEP Opposite() { return opposite; }
  BRepFP Face() { return face; }

  // set methods
  bool SetFrom(BRepPP p) { from = p; }
  bool SetTo(BRepPP p)  { to = p; }
  bool SetNext(BRepHEP e) { next = e; }
  bool SetOpposite(BRepHEP e) { opposite = e; }
  bool SetFace(BRepFP f) { face = f; }
};

class BRepLoop       : public BRepVisualItem
{
private:
  QVector<BRepHEP> hes;
  bool dir;

public:
  int HalfEdgeNum() { return hes.size(); }
  BRepHEP HalfEdge(int index)
  {
    if(index>=0 && index<hes.size()) return hes[index];
    else return BRepHEP(nullptr);
  }
  int AddHalfEdge(BRepHEP he) { hes.push_back(he); return hes.size()-1;}
  bool RemoveHalfEdge(int index)
  {
    if(index>=0 && index<hes.size())
    {
      hes.remove(index);
      return true;
    }
    else
      return false;
  }
  void Clear() { hes.clear(); }
  void SetDir(bool d) { dir = d; }
  bool Dir() { return dir; }

  bool VOnLoop(BRepPP v)
  {
    for(int i=0; i<hes.size(); i++)
    {
      if(hes[i]->From()==v)
        return ture;
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
    for(int i=0; i<hes.size(); i++)
    {
      if(hes[i]->From == v)
      {
        return hes[i];
      }
    }
    return BRepHEP(nullptr);
  }

  BRepHEP HEToV(BRepPP v)
  {
    for(int i=0; i<hes.size(); i++)
    {
      if(hes[i]->To() == v)
      {
        return hes[i];
      }
    }
    return BRepHEP(nullptr);
  }
};

class BRepFace       : public BRepVisualItem
{
private:
  BRepTP texture;
//  QVector<BRepRenderData> renderData;
  QVector<BRepLP> loops;
  BRepMP mesh;
  QVector3D normal;

public:
  // get methods
  BRepTP Texture() { return texture; }
  int LoopNum() { return loops.size(); }
  BRepLP Loop(int index) { return loops[index]; }
//  int RenderDataNum() { return renderData.size(); }
//  BRepRP RenderData(int index) { return renderData[index]; }
  BRepMP Mesh() { return mesh; }
  QVector3D Normal() { return normal; }

  // set methods
  bool SetTexture(BRepTP t) { texture = t; }
  int AddLoop(BRepLP loop) { loops.push_back(loop); return loops.size()-1;}
  bool RemoveLoop(int index) { loops.remove(index); }
//  bool AddRenderData(BRepRenderData r) { renderData.push_back(r); }
//  bool RemoveRenderData(int index) { renderData.remove(index); }
  bool SetMesh(BRepMP m) { mesh = m; }
  void SetNormal(QVector3D n) { normal = n; }
};

class BRepMesh       : public BRepVisualItem
{
private:
  QVector<BRepFace> faceLib;
  QVector<BRepPoint> pointLib;
  QVector<BRepHalfEdge> halfEdgeLib;
  QVector<BRepLoop> loopLib;
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
  bool RemoveFace(int index) { faceLib.remove(index); }

  int AddPoint(BRepPoint p) { pointLib.push_back(p); return pointLib.size()-1; }
  BRepPP AddPoint()
  {
    BRepPoint p;
    pointLib.push_back(p);
    return BRepPP(&pointLib[pointLib.size()-1]);
  }
  bool RemovePoint(int index) { pointLib.remove(index); }

  int AddHalfEdge(BRepHalfEdge e) { halfEdgeLib.push_back(e); return halfEdgeLib.size()-1; }
  BRepHEP AddHalfEdge()
  {
    BRepHEP he;
    halfEdgeLib.push_back(he);
    return BRepHEP(&halfEdgeLib[halfEdgeLib.size()-1]);
  }
  bool RemoveHalfEdge(int index) { halfEdgeLib.remove(index); }

  int AddLoop(BRepLoop l) { loopLib.push_back(l); return loopLib.size()-1; }
  BRepLP AddLoop()
  {
    BRepLoop l;
    loopLib.push_back(l);
    return BRepLP(&loopLib[loopLib.size()-1]);
  }
  bool RemoveLoop(int index) { halfEdgeLib.remove(index); }

  void SetName(QString n) { name = n; }
};

class BRepObject     : public BRepVisualItem
{
private:
  QString name;
  QString folder;
  QVector<BRepMesh> meshLib;
  QVector<BRepTexture> textureLib;

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
  bool RemoveMesh(int index) { meshLib.remove(index); }
//  bool AddTexture(BRepTexture text) { textureLib.push_back(text); }
  bool RemoveTexture(int index) { textureLib.remove(index); }
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
  QVector3D color;
  QVector2D texcoord;
  bool dir;
};

#endif // BREP_HPP
