#ifndef WIFI_H
#define WIFI_H

#include "i2c-display-lib.h"

#include "wifi.h"

void display_wifi(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void start_wifi()
{
	//setup buttons
	int button_up = 20;
	int button_ok = 19;
	int button_down = 18;
	
	pinMode(button_up, INPUT_PULLUP); //up
	pinMode(button_ok, INPUT_PULLUP); //ok
	pinMode(button_down, INPUT_PULLUP); //down
	
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
	
	lcd_clear();
	
	display_wifi("Module", 0);
	display_wifi("wifi", 0);
	
	delay(2000);
	return;
}

#endif