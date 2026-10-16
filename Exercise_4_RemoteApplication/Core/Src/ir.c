#include "main.h"
#include "lab_config.h"
#include "lab_logic.h"
#include "ir.h"
#if HAS_IR
static NECState decoder;
static volatile uint16_t tim6_count = 65535U;
static volatile uint8_t command_ok;
static volatile uint8_t irda_cmd[4];

void IrTick100us(void)
{
    if (tim6_count < 65535U) ++tim6_count;
    if (tim6_count > 150U) decoder.receiving = 0U;
}
void IrGetBitTime(void)
{
    uint16_t elapsed = tim6_count;
    uint8_t bytes[4];
    tim6_count = 0U;
    if (NECFeed(&decoder, elapsed, bytes) && !command_ok) {
        for (uint8_t i=0U; i<4U; ++i) irda_cmd[i] = bytes[i];
        command_ok = 1U;
    }
}
uint8_t IrReadCommand(uint8_t command[4])
{
    /* Tiny critical section prevents ISR/main from sharing a partial frame. */
    uint32_t saved = __get_PRIMASK();
    __disable_irq();
    uint8_t ready = command_ok;
    if (ready) {
        for (uint8_t i=0U; i<4U; ++i) command[i] = irda_cmd[i];
        command_ok = 0U;
    }
    __set_PRIMASK(saved);
    return ready;
}
void SerialInit(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio);
    /* APB1 is 45 MHz. Oversampling by 16: BRR = round(45000000/115200). */
    USART2->CR1 = 0U;
    USART2->CR2 = 0U;
    USART2->CR3 = 0U;
    USART2->BRR = (HAL_RCC_GetPCLK1Freq() + 57600U) / 115200U;
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}
void SerialPrintCommand(const uint8_t command[4])
{
    static const char hex[] = "0123456789ABCDEF";
    uint8_t text[10];
    for (uint8_t i=0U; i<4U; ++i) {
        text[2U*i] = (uint8_t)hex[command[i] >> 4U];
        text[2U*i+1U] = (uint8_t)hex[command[i] & 15U];
    }
    text[8] = '\r'; text[9] = '\n';
    uint32_t start = HAL_GetTick();
    for (uint8_t i=0U; i<sizeof text; ++i) {
        while ((USART2->SR & USART_SR_TXE) == 0U) {
            if ((uint32_t)(HAL_GetTick() - start) >= 10U) return;
        }
        USART2->DR = text[i];
    }
}
#endif
