#ifndef LAB_LOGIC_H
#define LAB_LOGIC_H
#include <stdint.h>
uint8_t LEDPattern(uint8_t mode, uint8_t step);
uint8_t LEDCycleLength(uint8_t mode);
uint8_t SegmentCode(uint8_t digit);
typedef struct { uint8_t stable, samples; } ButtonState;
uint8_t ButtonUpdate(ButtonState *state, uint8_t high);
typedef struct { uint8_t receiving, count, bytes[4]; } NECState;
/* One interval in 100 us ticks between consecutive falling edges. */
uint8_t NECFeed(NECState *state, uint16_t ticks, uint8_t out[4]);
#endif
