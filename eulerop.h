#ifndef EULEROP_H
#define EULEROP_H

#include "brep.hpp"

class EulerOp
{
public:
  static void MVFS(BRepOP *object, QVector3D pos);
  static void MEV(BRepMP mesh, BRepPP point);
};

#endif // EULEROP_H
