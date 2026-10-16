#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>
typedef struct {uint32_t BSRR;} GPIO_TypeDef;
extern GPIO_TypeDef gpioA,gpioD,gpioE,gpioG;
#define GPIOA (&gpioA)
#define GPIOD (&gpioD)
#define GPIOE (&gpioE)
#define GPIOG (&gpioG)
#define GPIO_PIN_0 1U
#define GPIO_PIN_2 4U
#define GPIO_PIN_3 8U
#define GPIO_PIN_5 32U
#define GPIO_PIN_14 16384U
#define GPIO_PIN_RESET 0U
#define GPIO_PIN_SET 1U
#define GPIO_MODE_OUTPUT_PP 1U
#define GPIO_MODE_IT_FALLING 2U
#define GPIO_NOPULL 0U
#define GPIO_PULLUP 1U
#define GPIO_SPEED_FREQ_LOW 0U
#define EXTI9_5_IRQn 23U
#define TIM6 ((void*)6)
typedef struct {uint32_t Pin,Mode,Pull,Speed;} GPIO_InitTypeDef;
typedef struct {void *Instance;} TIM_HandleTypeDef;
#define __HAL_RCC_GPIOD_CLK_ENABLE() ((void)0)
#define __HAL_RCC_GPIOE_CLK_ENABLE() ((void)0)
void HAL_GPIO_WritePin(GPIO_TypeDef*,uint32_t,uint32_t);
void HAL_GPIO_Init(GPIO_TypeDef*,GPIO_InitTypeDef*);
void HAL_NVIC_SetPriority(uint32_t,uint32_t,uint32_t);
void HAL_NVIC_EnableIRQ(uint32_t);
void HAL_GPIO_TogglePin(GPIO_TypeDef*,uint32_t);
uint32_t HAL_GetTick(void);
#endif
