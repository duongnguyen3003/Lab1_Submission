#ifndef IR_H
#define IR_H
#include <stdint.h>
void IrTick100us(void);
void IrGetBitTime(void);
uint8_t IrReadCommand(uint8_t command[4]);
void SerialInit(void);
void SerialPrintCommand(const uint8_t command[4]);
#endif
