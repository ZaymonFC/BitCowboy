typedef int bool;
enum { false, true };

typedef struct Position {
  int x;
  int y;
} Position;

typedef enum { top, right, bottom, left, none } Direction;

typedef struct {
  bool left;
  bool right;
  bool bottom;
} CollisionInfo;

typedef struct Rectangle {
  Position topLeft;
  Position topRight;
  Position bottomLeft;
  Position bottomRight;
} Rectangle;
