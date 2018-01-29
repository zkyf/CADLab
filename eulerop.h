#ifndef EULEROP_H
#define EULEROP_H

#include <QDebug>
#include "brep.hpp"

class EulerOp
{
public:
  static void MVFS(BRepOP &object, QVector3D pos);
  static BRepPP MEV(BRepMP mesh, BRepLP loop, BRepPP point, QVector3D pos);
  static BRepFP MEF(BRepMP mesh, BRepLP &loop, BRepPP v1, BRepPP v2);
  static BRepLP KEMR(BRepMP mesh, BRepLP loop, BRepHEP he);
  static void KFMRH(BRepMP mesh, BRepFP f1, BRepFP f2);
  static BRepFP Sweep(BRepMP mesh, BRepFP f1, float d, QVector3D n);
  static void Subdivide(BRepOP object, BRepOP& out);
};

#endif // EULEROP_H
