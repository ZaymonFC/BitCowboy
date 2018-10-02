#include "types.h"
#include "data.h"

extern void DelayMs(unsigned int);
extern void LCD_PutPixel(unsigned int, unsigned int, unsigned int);
extern void LCD_Contrast(int, int);
extern void LCD_Refresh(void);
extern void LCD_Blank(void);
extern void LCD_Init(void);
extern void LCD_CLEAR_MAT(void);
extern void SetLCDColour(int r, int g, int b);
extern int interpolate(int, int, int, int, int);


void CalculateViewPortCoordinates(Position *viewPort, Position *playerPosition) {
  viewPort->x = playerPosition->x - 61;
  // viewPort->y = playerPosition->y - 32;
  viewPort->y = 0;
}

void DrawPlayer(Position *playerPosition, Position *viewPort, Orientation orientation) {
  int screenX = playerPosition->x - viewPort->x;
  int screenY = playerPosition->y - viewPort->y;

  for (int x = 0; x < SPRITE_WIDTH; x++) {
    for (int y = 0; y < SPRITE_HEIGHT; y++) {
      int pixelValue = sprite[orientation][x + (y * SPRITE_WIDTH)];
      if (pixelValue)
        LCD_PutPixel(screenX + x, screenY + y, pixelValue);
    }
  }
}

void DrawTile(int tileNumber, int xPos, int yPos) {
  if (tileNumber == 3)
    return;
  for (int i = 0; i < 256; i++) {
    int x = i % 16 + 1 + xPos;
    int y = (i / 16) + 1 + yPos;
    LCD_PutPixel(x, y, tiles[tileNumber - 1][i]);
  }
}

void DrawMap(Position *viewPort, struct Map *map, const int MAP_TILES) {
  // 'Intelligently' determine the range of tiles to draw and the offset for
  // each tile. Calculate the tile coords of the top left tile on the screen
  int xOffset = viewPort->x;
  int yOffset = viewPort->y;

  int tileX = xOffset / 16;
  int tileY = yOffset / 16;

  // Calculate the per tile offset
  int tileOffsetX = xOffset % 16;
  int tileOffsetY = yOffset % 16;
  
  for (int row = 0; row < SCREEN_TILES_HEIGHT + 1; row++) {
    for (int col = 0; col < SCREEN_TILES_WIDTH + 1; col++) {
      int x = col * 16 - tileOffsetX;
      int y = row * 16 - tileOffsetY;

      // Calculate tile type based on tile position
      int tile = (col + tileX) + ((row + tileY) * map->width);

      // Sanity Check the Tile (Allow viewport over map boundary)
      if (tile < 0 || tile >= MAP_TILES) {
        continue;
      }
      if (tileX + col < 0 || tileX + col >= map->width) {
        continue;
      }
      if (tileY + row < 0 || tileY + row >= map->height) {
        continue;
      }

      // Draw the tile at screen offset x, y
      // LCD_PutPixel(tile, 10, 1);
      DrawTile(map->tiles[tile], x, y);
    }
  }
}

void DrawLetter(int x, int y, int letter) {
  for (int r = 0; r < 7; r++) {
    for (int c = 0; c < 5; c++) {
      int tile = c + r * 5;
      LCD_PutPixel(x + c, y + r, alphabet[letter][tile]);
    }
  }  
}

void DrawText(int x, int y, char * text, int length) {
  int textX = x;
  for (int i = 0; i < length; i++) {
    int letter = (int)text[i] - 0x60;
    DrawLetter(textX, y, letter);
    textX += 6;
  }
}
