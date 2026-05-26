#ifndef PRIMITIVESHAPE_H
#define PRIMITIVESHAPE_H

#include "entity.h"
#include <vector>

enum ShapeType { CIRCLE, TRIANGLE, SQUARE };

class PrimitiveShape : public Entity {
public:
  ShapeType type;

  PrimitiveShape(ShapeType t, float x, float y);

  void draw() override;
  void drawHandles() override;

  bool containsPoint(float px, float py) override;
  int getHandleHit(float px, float py) override;
  int getEntityType() override { return 4; }
};
struct HandlesOut {
  Point hX, hY, hScale, hRotate;
};

HandlesOut getHandleCenters(const PrimitiveShape *s);

#endif // PRIMITIVESHAPE_H
