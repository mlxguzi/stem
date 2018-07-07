
sbit key_input = P1^1;

#define key_no	   0    //no keys
#define key_click  1    //click keys
#define key_double 2    //double click
#define key_long   3    //long click

#define key_state_0		0
#define key_state_1   1
#define key_state_2   2
#define key_state_3   3 //key states define

#define key_flag	(1<<0)
#define clear_key_flag  (~(1<<0))

#define key_init()	(key_input = 1)
extern UINT8 key_read();



static unsigned char key_driver(void);
unsigned char key_read(void);