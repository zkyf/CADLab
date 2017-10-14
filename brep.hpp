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

#ifndef QSP
#define QSP QSharedDataPointer
#endif

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

typedef QSP<BRepPoint>      BRepPP;
typedef QSP<BRepHalfEdge>   BRepHEP;
typedef QSP<BRepFace>       BRepFP;
typedef QSP<BRepMesh>       BRepMP;
typedef QSP<BRepObject>     BRepOP;
typedef QSP<BRepTexture>    BRepTP;
typedef QSP<BRepRenderData> BRepRP;

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
  QVector3D Position();
  QVector3D Normal();
  int fanOutNum();
  BRepHEP FanOutEdge(int);

  // set methods
  void SetPosition(QVector3D);
  void SetNormal(QVector3D);
  bool AddFanOutEdge(BRepHEP);
  bool RemoveFanOutEdge(int);
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
  BRepPP From();
  BRepPP To();
  BRepHEP Next();
  BRepHEP Opposite();
  BRepFP GetFace();

  // set methods
  bool SetFrom(BRepPP);
  bool SetTo(BRepPP);
  bool SetNext(BRepHEP);
  bool SetOpposite(BRepHEP);
  bool SetFace(BRepFP);
};

class BRepFace       : public BRepVisualItem
{
private:
  BRepTP texture;
  QVector<BRepRP> renderData;
  BRepMP mesh;
  QVector3D normal;
};

#endif // BREP_HPP
