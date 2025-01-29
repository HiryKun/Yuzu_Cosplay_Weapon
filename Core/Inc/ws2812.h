#ifndef __WS2812_H
#define __WS2812_H

#define WS2812_BIT1   57
#define WS2812_BIT0   22
#define WS2812_RESET  0
#define LED_NUM       504
#define BACKG 0,0,3
#define BLACK 0,0,0
#define WHITE 50,50,50
#define MAGENTA 70,10,70
#define CYAN 0,80,100
#define RED 100,0,0
#define ORANGE 100,35,0
#define GREEN 0,100,0
#define BLUE 0,0,100
#define PURPLE 70,0,70

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}WS2812_RGB;

void WS2812_Update();
void DrawStar(uint8_t col, uint8_t row, uint8_t mode);
void WS2812_RGB2Buffer(WS2812_RGB rgb[], uint8_t buffer[]);
void WS2812_WriteColor(uint16_t index, uint8_t R, uint8_t G, uint8_t B);

#endif