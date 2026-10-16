#include "main.h"
#include "lab_config.h"
#include "lab_logic.h"
#include "leds.h"
#if HAS_LEDS
static uint8_t step;
void ResetLEDEffect(void) { step = 0U; }
void DisplayLEDs(int mode)
{
    uint8_t LED_Value = LEDPattern((uint8_t)mode, step);
    uint32_t pins = (uint32_t)LED_Value << 8U;
    GPIOD->BSRR = ((0xFF00U & ~pins) << 16U) | pins;
    step = (uint8_t)((step + 1U) % LEDCycleLength((uint8_t)mode));
}
#endif
