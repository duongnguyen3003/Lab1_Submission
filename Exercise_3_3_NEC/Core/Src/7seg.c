#include "main.h"
#include "lab_config.h"
#include "lab_logic.h"
#include "7seg.h"
#if HAS_7SEG
static volatile uint16_t display_digits;
void Set7SegDisplayValue(uint8_t value)
{
    value %= 100U;
    display_digits = (uint16_t)(((value / 10U) << 8U) | (value % 10U));
}
void Set7SegHexValue(uint8_t value)
{
    display_digits = (uint16_t)(((value >> 4U) << 8U) | (value & 15U));
}
void Run7SegDisplay(void)
{
    static uint8_t digit;
    static uint16_t frame;
    /* Blank both digits before changing shared segments to prevent ghosting. */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
    if (digit == 0U) frame = display_digits;
    uint8_t nibble = digit == 0U ? (uint8_t)(frame >> 8U) : (uint8_t)frame;
    uint32_t pins = (uint32_t)SegmentCode(nibble) << 8U;
    GPIOE->BSRR = ((0xFF00U & ~pins) << 16U) | pins;
    HAL_GPIO_WritePin(GPIOG, digit == 0U ? GPIO_PIN_2 : GPIO_PIN_3, GPIO_PIN_SET);
    digit ^= 1U;
}
#endif
