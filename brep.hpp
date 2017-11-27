/*
 * Created on Oct 14 2017
 * This is done by Jiaxin Liu as part of the final project for
 * the CAD designing course in Zhejiang University
 */

#ifndef BREP_HPP
#define BREP_HPP

#include <QSharedData>
#include <QSharedDataPointer>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

// list of all classes
class BRepItem;
class BRepVisualItem;
class BRepPoint;
class BRepHalfEdge;
class BRepFace;
class BRepMesh;
class BRepObject;
class BRepTexture;
class BRepRenderData;

#ifndef QSP
#define QSP QExplicitlySharedDataPointer
#endif

typedef QSP<BRepPoint>      BRepPP;
typedef QSP<BRepHalfEdge>   BRepHEP;
typedef QSP<BRepFace>       BRepFP;
typedef QSP<BRepMesh>       BRepMP;
typedef QSP<BRepObject>     BRepOP;
typedef QSP<BRepTexture>    BRepTP;
typedef QSP<BRepRenderData> BRepRP;

typedef QVector<BRepPoint>::iterator PointIte;
typedef QVector<BRepHalfEdge>::iterator HEIte;
typedef QVector<BRepFace>::iterator FaceIte;
typedef QVector<BRepMesh>::iterator MeshIte;
typedef QVector<BRepTexture>::iterator TextureIte;
typedef QVector<BRepRenderData>::iterator RDIte;

// class definiations
class BRepItem       : public QSharedData
{
public:
  bool valid;

public:
  BRepItem(bool _valid=false) : valid(_valid) {}
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
  int fanOutNum() { return fanOutList.size(); }
  BRepHEP FanOutEdge(int index) { return fanOutList[index]; }

  // set methods
  void SetPosition(QVector3D pos) { position = pos; }
  void SetNormal(QVector3D n) { normal = n; }
  bool AddFanOutEdge(BRepHEP edge) { fanOutList.push_back(edge); }
  bool RemoveFanOutEdge(int index) { fanOutList.remove(index); }
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

class BRepFace       : public BRepVisualItem
{
private:
  BRepTP texture;
  QVector<BRepRenderData> renderData;
  BRepMP mesh;
  QVector3D normal;

public:
  // get methods
  BRepTP Texture() { return texture; }
  int RenderDataNum() { return renderData.size(); }
  BRepRP RenderData(int index) { return renderData[index]; }
  BRepMP Mesh() { return mesh; }
  QVector3D Normal() { return normal; }

  // set methods
  bool SetTexture(BRepTP t) { texture = t; }
  bool AddRenderData(BRepRenderData r) { renderData.push_back(r); }
  bool RemoveRenderData(int index) { renderData.remove(index); }
  bool SetMesh(BRepMP m) { mesh = m; }
  void SetNormal(QVector3D n) { normal = n; }
};

class BRepMesh       : public BRepVisualItem
{
private:
  QVector<BRepFace> faceLib;
  QVector<BRepPoint> pointLib;
  QVector<BRepHalfEdge> halfEdgeLib;
  BRepOP object;
  QString name;

public:
  // get methods
  int FaceNum() { return faceLib.size(); }
  BRepFP Face(int index) { return &faceLib[index]; }
  int PointNum() { return pointLib.size(); }
  BRepPP Point(int index) { return &pointLib[index]; }
  int HalfEdgeNum() { return halfEdgeLib.size(); }
  BRepHEP HalfEdge(int index) { return &halfEdgeLib[index]; }
  BRepOP Object() { return object; }
  QString Name() { return name; }

  // set methods
  bool AddFace(BRepFace f) { faceLib.push_back(f); }
  bool RemoveFace(int index) { faceLib.remove(index); }
  bool AddPoint(BRepPoint p) { pointLib.push_back(p); }
  bool RemovePoint(int index) { pointLib.remove(index); }
  bool AddHalgEdge(BRepHalfEdge e) { halfEdgeLib.push_back(e); }
  bool RemoveHalfEdge(int index) { halfEdgeLib.remove(index); }
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
  int GetMeshNum() { return meshLib.size(); }
  BRepMP Mesh(int index) { return meshLib.size(); }
  int TextureNum() { return textureLib.size(); }
  BRepTP Texture(int index) { return &textureLib[index]; }

  // set methods
  bool AddMesh(BRepMesh m) { meshLib.push_back(m); }
  bool RemoveMesh(int index) { meshLib.remove(index); }
  bool AddTexture(BRepTexture t) { textureLib.push_back(t); }
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
};

#endif // BREP_HPP
