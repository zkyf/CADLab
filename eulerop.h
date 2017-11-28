#ifndef EULEROP_H
#define EULEROP_H

#include "brep.hpp"

class EulerOp
{
public:
  static void MVFS(BRepOP *object, QVector3D pos);
  static BRepPP MEV(BRepMP mesh, BRepLP loop, BRepPP point, QVector3D pos);
  static BRepLP MEL(BRepMP mesh, BRepLP loop, BRepPP v1, BRepPP v2);
};

#endif // EULEROP_H
