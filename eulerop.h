#ifndef EULEROP_H
#define EULEROP_H

#include "brep.hpp"

class EulerOp
{
public:
  static void MVFS(BRepOP *object);
  static void MEV(BRepMP mesh, BRepPP point);
};

#endif // EULEROP_H
