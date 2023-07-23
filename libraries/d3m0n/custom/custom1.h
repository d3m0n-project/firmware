#ifndef CUSTOM1_H
#define CUSTOM1_H

#include "i2c-display-lib.h"
#include "settings.h"
#include <string.h>
#include <SPI.h>
#include <SD.h>

//setup buttons
#define button_up 22
#define button_ok 21
#define button_down 20



void display_custom1(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void start_custom1()
{
	#define FINISHED 1
	
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
	
	lcd_clear();
	
	display_rfid("Module", 0);
	display_rfid("rfid", 0);
	
	
	#define FINISHED 0
}

#endif