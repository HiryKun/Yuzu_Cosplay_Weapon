#include "stm32f1xx_hal.h"
#include "ws2812.h"
#include <stdbool.h>

extern  TIM_HandleTypeDef   htim1;
extern  TIM_HandleTypeDef   htim2;
#define WS2812_UpdateTIM    htim2
#define WS2812_DataTIM      htim1
#define WS2812_DataTIM_CH   TIM_CHANNEL_1

uint8_t ws2812_buffer[24 * LED_NUM + 1] = {0};
WS2812_RGB leds[LED_NUM];
bool ws2812_need_update = true;
uint8_t nyan_cat_index = 0;
uint8_t star_index = 0;
bool cat_frame = true;
const uint8_t nyan_cat[3][LED_NUM * 3] = {
    {BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, BLACK, BLACK, BLACK, MAGENTA, WHITE, BLACK, BLACK, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, RED, BACKG, BACKG, BACKG, BACKG, BACKG, RED, RED, RED, RED, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, BLACK, BLACK, BLACK, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, ORANGE, RED, RED, RED, RED, RED, ORANGE, ORANGE, ORANGE, ORANGE, RED, RED, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, MAGENTA, WHITE, WHITE, WHITE, CYAN, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, GREEN, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, GREEN, GREEN, GREEN, GREEN, ORANGE, ORANGE, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, MAGENTA, WHITE, WHITE, WHITE, CYAN, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BLUE, GREEN, GREEN, GREEN, GREEN, GREEN, BLUE, BLUE, BLUE, BLUE, GREEN, GREEN, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, BLACK, WHITE, BLACK, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, WHITE, BLACK, BLUE, BLUE, BLUE, BLUE, PURPLE, PURPLE, PURPLE, PURPLE, BLUE, BLUE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, BACKG, BACKG, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, PURPLE, PURPLE, PURPLE, PURPLE, BACKG, BACKG, BACKG, BACKG, PURPLE, PURPLE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BACKG, BACKG, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BACKG, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG},
    {BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, BLACK, BLACK, BLACK, MAGENTA, WHITE, BLACK, BLACK, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BACKG, BACKG, RED, RED, RED, RED, BACKG, BACKG, BACKG, BACKG, BACKG, RED, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, BLACK, BLACK, BLACK, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, RED, RED, ORANGE, ORANGE, ORANGE, ORANGE, RED, RED, RED, RED, RED, ORANGE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, MAGENTA, WHITE, WHITE, WHITE, CYAN, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, ORANGE, ORANGE, GREEN, GREEN, GREEN, GREEN, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, GREEN, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, MAGENTA, WHITE, WHITE, WHITE, CYAN, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, GREEN, GREEN, BLUE, BLUE, BLUE, BLUE, GREEN, GREEN, GREEN, GREEN, GREEN, BLUE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, BLACK, WHITE, BLACK, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, BLUE, PURPLE, PURPLE, PURPLE, PURPLE, BLUE, BLUE, BLUE, BLUE, BLUE, PURPLE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, BACKG, BACKG, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BACKG, BACKG, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BACKG, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG},
    {BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, RED, RED, RED, RED, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, BLACK, BLACK, BLACK, MAGENTA, WHITE, BLACK, BLACK, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, RED, RED, RED, RED, ORANGE, ORANGE, ORANGE, ORANGE, RED, RED, RED, RED, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, BLACK, BLACK, BLACK, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, ORANGE, ORANGE, ORANGE, ORANGE, GREEN, GREEN, GREEN, GREEN, ORANGE, ORANGE, ORANGE, ORANGE, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, MAGENTA, WHITE, WHITE, WHITE, CYAN, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, MAGENTA, WHITE, WHITE, WHITE, CYAN, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, GREEN, GREEN, GREEN, GREEN, BLUE, BLUE, BLUE, BLUE, GREEN, GREEN, GREEN, GREEN, 
    BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, MAGENTA, WHITE, WHITE, BLACK, WHITE, BLACK, WHITE, WHITE, CYAN, BLACK, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, CYAN, BLACK, BLUE, BLUE, BLUE, BLUE, PURPLE, PURPLE, PURPLE, PURPLE, BLUE, BLUE, BLUE, BLUE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, BLACK, PURPLE, PURPLE, PURPLE, BACKG, BACKG, BACKG, BACKG, PURPLE, PURPLE, PURPLE, PURPLE, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, BACKG, BACKG, BLACK, WHITE, WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BLACK, BLACK, BACKG, BACKG, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BLACK, BLACK, BLACK, BACKG, BLACK, BLACK, BLACK, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, 
    BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG, BACKG}
};

void WS2812_Update() {
    if (!ws2812_need_update) return;
    ws2812_need_update = false;
    for (uint16_t i = 0; i < LED_NUM; ++i) {
        WS2812_WriteColor(i,    nyan_cat[nyan_cat_index][i * 3],
                                nyan_cat[nyan_cat_index][i * 3 + 1],
                                nyan_cat[nyan_cat_index][i * 3 + 2]);
    }
    DrawStar(2, 2, star_index);
    DrawStar(5, 11, (star_index + 2) % 4);
    DrawStar(30, 2, (star_index + 2) % 4);
    if (cat_frame) nyan_cat_index = (nyan_cat_index + 1) % 3;
    else star_index = (star_index + 1) % 4;
    cat_frame ^= 1;
    WS2812_RGB2Buffer(leds, ws2812_buffer);
    HAL_TIM_PWM_Start_DMA(&WS2812_DataTIM, WS2812_DataTIM_CH, (uint32_t *)&ws2812_buffer, sizeof(ws2812_buffer));
}

void DrawStar(uint8_t col, uint8_t row, uint8_t mode) {
    if (mode >= 2) return;
    uint8_t back_color[3] = {BACKG};
    uint16_t index = (uint16_t)row * 36 + col;
    if (leds[index].r == back_color[0] && leds[index].g == back_color[1] && leds[index].b == back_color[2])
        WS2812_WriteColor(index, WHITE);
    uint16_t operate[4] = {index - 1 - mode, index + 1 + mode, index - 36 * (mode + 1), index + 36 * (mode + 1)};
    for (uint8_t i = 0; i < 4; ++i) {
        if (operate[i] < 0 || operate[i] >= LED_NUM) continue;
        if (leds[operate[i]].r == back_color[0] && leds[operate[i]].g == back_color[1] && leds[operate[i]].b == back_color[2])
            WS2812_WriteColor(operate[i], WHITE);
    }
}

void WS2812_RGB2Buffer(WS2812_RGB rgb[], uint8_t buffer[]) {
    for (uint16_t i = 0; i < LED_NUM; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
        buffer[24 * i + j]        = (rgb[i].g & (0x80 >> j)) ? WS2812_BIT1 : WS2812_BIT0;
        buffer[24 * i + j + 8]    = (rgb[i].r & (0x80 >> j)) ? WS2812_BIT1 : WS2812_BIT0;
        buffer[24 * i + j + 16]   = (rgb[i].b & (0x80 >> j)) ? WS2812_BIT1 : WS2812_BIT0;
        }
    }
}

inline void WS2812_WriteColor(uint16_t index, uint8_t R, uint8_t G, uint8_t B) {
    leds[index].r = R;
    leds[index].g = G;
    leds[index].b = B;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &WS2812_UpdateTIM) {
        ws2812_need_update = true;
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(&WS2812_DataTIM, WS2812_DataTIM_CH);
}