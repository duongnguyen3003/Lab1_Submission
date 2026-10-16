#include "lab_logic.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint8_t frame(NECState *s, const uint8_t b[4], uint8_t out[4], int jitter){
 uint8_t ready=0; assert(!NECFeed(s,135,out));
 for(int i=0;i<32;i++) {
  int bit=(b[i/8]>>(i%8))&1;
  ready=NECFeed(s,(uint16_t)((bit?23:11)+jitter),out);
  if(i<31) assert(!ready);
 } return ready;
}
int main(void){
 const uint8_t l[]={0,1,2,4,8,16,32,64,128,0};
 const uint8_t r[]={0,128,64,32,16,8,4,2,1,0};
 const uint8_t b[]={0,129,66,36,24,24,36,66,129,0};
 for(int i=0;i<10;i++){assert(LEDPattern(1,i)==l[i]);assert(LEDPattern(2,i)==r[i]);assert(LEDPattern(4,i)==b[i]);}
 assert(LEDPattern(3,0)==0 && LEDPattern(3,1)==255 && LEDPattern(3,2)==0);
 assert(LEDCycleLength(1)==10 && LEDCycleLength(2)==10 && LEDCycleLength(3)==3 && LEDCycleLength(4)==10);
 assert(LEDPattern(0,0)==0 && LEDPattern(9,0)==0);
 assert(SegmentCode(0)==0x3f && SegmentCode(2)==0x5b && SegmentCode(5)==0x6d && SegmentCode(15)==0x71);
 ButtonState bs={0}; int presses=0;
 for(int i=0;i<10;i++) presses+=ButtonUpdate(&bs,i%2);
 for(int i=0;i<100;i++) presses+=ButtonUpdate(&bs,1);
 assert(presses==1);
 for(int i=0;i<20;i++) presses+=ButtonUpdate(&bs,0);
 for(int i=0;i<20;i++) presses+=ButtonUpdate(&bs,1);
 assert(presses==2);
 NECState s={0};uint8_t out[4]={0};const uint8_t valid[]={0,255,0x0c,0xf3};
 assert(frame(&s,valid,out,0)); assert(!memcmp(valid,out,4));
 assert(frame(&s,valid,out,-2)); assert(frame(&s,valid,out,2));
 const uint8_t badcmd[]={0,255,0x0c,0xf4}; assert(!frame(&s,badcmd,out,0));
 const uint8_t badaddr[]={1,255,0x0c,0xf3}; assert(!frame(&s,badaddr,out,0));
 assert(!NECFeed(&s,135,out)); assert(!NECFeed(&s,17,out));assert(!s.receiving);
 assert(!NECFeed(&s,112,out));assert(!s.receiving); /* repeat leader ignored */
 assert(!NECFeed(&s,135,out)); assert(!NECFeed(&s,11,out)); assert(!NECFeed(&s,600,out));assert(!s.receiving);
 assert(frame(&s,valid,out,0)); /* recovery */
 for(int cmd=0;cmd<256;cmd++){uint8_t v[]={0x34,0xcb,(uint8_t)cmd,(uint8_t)~cmd};assert(frame(&s,v,out,0));assert(!memcmp(v,out,4));}
 puts("PASS: LED sequences, lengths, segment codes, debounce, 256 NEC commands, jitter, invalid complements, repeat, timeout and recovery");
}
