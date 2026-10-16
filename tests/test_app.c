#include "main.h"
#include "app.h"
#include "lab_config.h"
#include <assert.h>
#include <stdio.h>
GPIO_TypeDef gpioA,gpioD,gpioE,gpioG;
static uint32_t now,toggles;
static uint8_t available,cmd[4],displayed,lastmode,resets,prints;
void HAL_GPIO_WritePin(GPIO_TypeDef*p,uint32_t a,uint32_t b){(void)p;(void)a;(void)b;}
void HAL_GPIO_Init(GPIO_TypeDef*p,GPIO_InitTypeDef*g){(void)p;(void)g;}
void HAL_NVIC_SetPriority(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;}
void HAL_NVIC_EnableIRQ(uint32_t a){(void)a;}
void HAL_GPIO_TogglePin(GPIO_TypeDef*p,uint32_t pins){assert(p==GPIOG && pins==GPIO_PIN_14);toggles++;}
uint32_t HAL_GetTick(void){return now;}
void SerialInit(void){}
void SerialPrintCommand(const uint8_t *v){assert(v[2]==cmd[2]);prints++;}
uint8_t IrReadCommand(uint8_t*v){if(!available)return 0;for(int i=0;i<4;i++)v[i]=cmd[i];available=0;return 1;}
void IrGetBitTime(void){}
void IrTick100us(void){}
void Set7SegHexValue(uint8_t v){displayed=v;}
void Run7SegDisplay(void){}
void DisplayLEDs(int mode){lastmode=(uint8_t)mode;}
void ResetLEDEffect(void){resets++;}
static void send(uint8_t value){cmd[0]=0;cmd[1]=255;cmd[2]=value;cmd[3]=(uint8_t)~value;available=1;now+=150;AppRun();assert(displayed==value);}
int main(void){
 AppInit();assert(displayed==0);
 send(REMOTE_KEY_1);assert(lastmode==1 && resets==1 && toggles==0);
 send(REMOTE_KEY_2);assert(lastmode==2 && resets==2 && toggles==0);
 send(REMOTE_KEY_3);assert(lastmode==3 && resets==3 && toggles==0);
 send(REMOTE_KEY_4);assert(lastmode==4 && resets==4 && toggles==0);
 send(REMOTE_KEY_OK);assert(toggles==1 && lastmode==4 && resets==4);
 send(0xAB);assert(displayed==0xAB && toggles==1 && lastmode==4 && resets==4);
 AppRun();assert(toggles==1 && prints==6);
 send(REMOTE_KEY_OK);assert(toggles==2);
 puts("PASS: Exercise 4 command byte display, four mode selections, unknown key, single consumption and LED4 toggled only by OK");
}
