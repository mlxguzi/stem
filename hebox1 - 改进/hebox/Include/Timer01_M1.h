#ifndef	__TIMER01_M1_H__
#define __TIMER01_M1_H__

#include "WS281X.h"

#define TH0_INIT        1300 
#define TL0_INIT        1300


U8	u8TH0_Tmp = (65536-TH0_INIT)/256;
U8  u8TL0_Tmp = (65536-TL0_INIT)%256;


#endif