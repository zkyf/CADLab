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
  static BRepLP KEMR(BRepMP mesh, BRepLP loop, BRepFP r, BRepHEP he);
  static void KFMRH(BRepMP mesh, BRepFP f1, BRepFP f2);
  static void Sweep(BRepMP mesh, BRepFP face, float d);
};

#endif // EULEROP_H
