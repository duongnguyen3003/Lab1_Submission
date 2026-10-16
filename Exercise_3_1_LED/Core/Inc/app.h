#ifndef APP_H
#define APP_H
#include <stdint.h>
void AppInit(void);
void AppRun(void);
void AppTick100us(void);
void AppEXTI(uint16_t pin);
#endif
