#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "key.h"


static unsigned char key_driver(void)
{
	static unsigned char key_state_buffer1 = key_state_0;
	static unsigned char key_timer_cnt1 = 0;
	unsigned char key_return = key_no;
	unsigned char key;
	
	key = key_input;  //read the I/O states
	
	switch(key_state_buffer1)
	{
		case key_state_0:
			if(key == 0)
				key_state_buffer1 = key_state_1; 
				
			break;
		case key_state_1:
			if(key == 0)
			{
				key_timer_cnt1 = 0;
				key_state_buffer1 = key_state_2;						
			}
			else
				key_state_buffer1 = key_state_0;
				
			break;  
		case key_state_2:
			if(key == 1) 
			{
				key_return = key_click;  
				key_state_buffer1 = key_state_0;  
			}
			else if(++key_timer_cnt1 >= 100)  
			{
				key_return = key_long;  
				key_state_buffer1 = key_state_3;  
			}
			break;
		case key_state_3:  
			if(key == 1) 
				key_state_buffer1 = key_state_0;  
			break;
	}
	return key_return;
}


unsigned char key_read(void)
{
	static unsigned char key_state_buffer2 = key_state_0;
	static unsigned char key_timer_cnt2 = 0;
	unsigned char key_return = key_no;
	unsigned char key;
	
	key = key_driver();
	
	switch(key_state_buffer2)
	{
		case key_state_0:
			if(key == key_click)
			{
				key_timer_cnt2 = 0;  
				key_state_buffer2 = key_state_1;
			}
			else 
				key_return = key;  
			break;
		case key_state_1:
			if(key == key_click)  
			{
				key_return = key_double;  
				key_state_buffer2 = key_state_0;
			}
			else if(++key_timer_cnt2 >= 50)
			{
									
				key_return = key_click;  
				key_state_buffer2 = key_state_0;  
					
			}
			break;
	}
	
	return key_return;
}