#define numLEDs 1
sbit LED_input = P0^6 ;

#define TH0_INIT        20000 
#define TL0_INIT        20000


#define LED_RING_DATE_PIN_LOW() (LED_input = 0) 
#define LED_RING_DATE_PIN_HIGH() (LED_input = 1) 




void H1_nop_800ns();
void H0_nop_400ns();
void Din_1(void) ;
void Din_0(void);
void Send_8bits(uint8_t dat);
void Send_2811_24bits(uint8_t RData, uint8_t GData, uint8_t BData);
void rst();
void ring_set_color_all_same(uint8_t r, uint8_t g, uint8_t b) ;
void ring_display_clear();
void led_open(UINT16 flag,uint8_t r, uint8_t g, uint8_t b);
 