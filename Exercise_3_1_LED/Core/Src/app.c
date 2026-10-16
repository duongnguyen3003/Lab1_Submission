#include "main.h"
#include "lab_config.h"
#include "lab_logic.h"
#include "app.h"
#include "leds.h"
#include "7seg.h"
#include "ir.h"
#if LAB_EXERCISE == 32
static volatile uint8_t countValue = 25U;
static volatile uint8_t button_armed;
static ButtonState button;
#endif
#if HAS_LEDS
static uint8_t mode = 1U;
#endif
void AppInit(void)
{
    GPIO_InitTypeDef gpio = {0};
#if HAS_LEDS
    __HAL_RCC_GPIOD_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOD, 0xFF00U, GPIO_PIN_RESET);
    gpio.Pin = 0xFF00U;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &gpio);
#endif
#if HAS_7SEG
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOE, 0xFF00U, GPIO_PIN_RESET);
    gpio.Pin = 0xFF00U;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &gpio);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOG, &gpio);
#if LAB_EXERCISE == 32
    Set7SegDisplayValue(countValue);
#else
    Set7SegHexValue(0U);
#endif
#endif
#if HAS_IR
    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_IT_FALLING;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOG, &gpio);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1U, 0U);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    SerialInit();
#else
    (void)gpio;
#endif
}
void AppTick100us(void)
{
#if HAS_IR
    IrTick100us();
#endif
#if HAS_7SEG
    static uint8_t divider;
    if (++divider == 10U) {
        divider = 0U;
        Run7SegDisplay();
#if LAB_EXERCISE == 32
        if (ButtonUpdate(&button,HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET) && button_armed) {
            button_armed = 0U;
            countValue = (uint8_t)((countValue + 1U) % 100U);
            Set7SegDisplayValue(countValue);
        }
#endif
    }
#endif
}
void AppEXTI(uint16_t pin)
{
#if HAS_IR
    if (pin == GPIO_PIN_5) IrGetBitTime();
#endif
#if LAB_EXERCISE == 32
    if (pin == GPIO_PIN_0) button_armed = 1U;
#endif
    (void)pin;
}
void AppRun(void)
{
#if HAS_IR
    uint8_t command[4];
    if (IrReadCommand(command)) {
        SerialPrintCommand(command);
#if LAB_EXERCISE == 4
        /* Display the actual third byte, not the human-readable key label. */
        Set7SegHexValue(command[2]);
        switch (command[2]) {
        case REMOTE_KEY_OK: HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14); break;
        case REMOTE_KEY_1: mode = 1U; ResetLEDEffect(); break;
        case REMOTE_KEY_2: mode = 2U; ResetLEDEffect(); break;
        case REMOTE_KEY_3: mode = 3U; ResetLEDEffect(); break;
        case REMOTE_KEY_4: mode = 4U; ResetLEDEffect(); break;
        default: break;
        }
#endif
    }
#endif
#if HAS_LEDS
    static uint32_t last;
#if LAB_EXERCISE == 32
    const uint32_t delay = 5U;
#else
    const uint32_t delay = LED_STEP_MS;
#endif
    uint32_t now = HAL_GetTick();
    if ((uint32_t)(now - last) >= delay) {
        last = now;
        DisplayLEDs(mode);
#if LAB_EXERCISE == 31
        static uint8_t steps, cycles;
        if (++steps == LEDCycleLength(mode)) {
            steps = 0U;
            if (++cycles == 2U) {
                cycles = 0U;
                mode = (uint8_t)(mode % 4U + 1U);
                ResetLEDEffect();
            }
        }
#endif
    }
#endif
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) AppTick100us();
}
void HAL_GPIO_EXTI_Callback(uint16_t pin) { AppEXTI(pin); }
