#include "lab_logic.h"
uint8_t LEDCycleLength(uint8_t mode)
{
    return (mode == 3U) ? 3U : ((mode >= 1U && mode <= 4U) ? 10U : 1U);
}
uint8_t LEDPattern(uint8_t mode, uint8_t step)
{
    static const uint8_t bumper[10] = {0,0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81,0};
    step %= LEDCycleLength(mode);
    switch (mode) {
    case 1: return (step >= 1U && step <= 8U) ? (uint8_t)(1U << (step-1U)) : 0U;
    case 2: return (step >= 1U && step <= 8U) ? (uint8_t)(0x80U >> (step-1U)) : 0U;
    case 3: return step == 1U ? 0xFFU : 0U;
    case 4: return bumper[step];
    default: return 0U;
    }
}
uint8_t SegmentCode(uint8_t digit)
{
    /* PE8=a, PE9=b, ..., PE14=g; bit 7 decimal point remains off. */
    static const uint8_t codes[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                                     0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    return codes[digit & 15U];
}
uint8_t ButtonUpdate(ButtonState *state, uint8_t high)
{
    high = high != 0U;
    if (high == state->stable) { state->samples = 0U; return 0U; }
    if (++state->samples < 20U) return 0U;
    state->samples = 0U;
    state->stable = high;
    return high; /* One event after 20 consecutive high samples; rearm on release. */
}
uint8_t NECFeed(NECState *state, uint16_t ticks, uint8_t out[4])
{
    if (ticks >= 120U && ticks <= 150U) {
        state->receiving = 1U; state->count = 0U;
        for (uint8_t i=0U; i<4U; ++i) state->bytes[i] = 0U;
        return 0U;
    }
    if (!state->receiving) return 0U;
    uint8_t bit;
    if (ticks >= 8U && ticks <= 14U) bit = 0U;
    else if (ticks >= 18U && ticks <= 27U) bit = 1U;
    else { state->receiving = 0U; return 0U; }
    state->bytes[state->count / 8U] |= (uint8_t)(bit << (state->count % 8U));
    if (++state->count != 32U) return 0U;
    state->receiving = 0U;
    if ((uint8_t)(state->bytes[0] ^ state->bytes[1]) != 0xFFU ||
        (uint8_t)(state->bytes[2] ^ state->bytes[3]) != 0xFFU) return 0U;
    for (uint8_t i=0U; i<4U; ++i) out[i] = state->bytes[i];
    return 1U;
}
