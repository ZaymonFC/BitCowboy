typedef int bool;
enum { false, true };

typedef struct Position {
  int x;
  int y;
} Position;

typedef enum { top, right, bottom, left, none } Direction;

typedef struct Map {
  int width;
  int height;
  char tiles[128];
} Map;

typedef struct CollisionInfo{
  bool left;
  char leftTile;
  bool right;
  char rightTile;
  bool bottom;
  char bottomTile;
} CollisionInfo;

typedef struct Rectangle {
  Position topLeft;
  Position topRight;
  Position bottomLeft;
  Position bottomRight;
} Rectangle;

typedef enum { idle, l, r } Orientation;
