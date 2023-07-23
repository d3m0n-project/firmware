#ifndef RFID_H
#define RFID_H

#include "i2c-display-lib.h"

#include "rfid.h"

//setup buttons
#define button_up 22
#define button_ok 21
#define button_down 20

void display_rfid(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void start_rfid()
{
	#define FINISHED 1
	
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
	
	lcd_clear();
	
	display_rfid("Module", 0);
	display_rfid("rfid", 0);
	
	delay(2000);
	#define FINISHED 0
}

#endif