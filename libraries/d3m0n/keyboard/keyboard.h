#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "i2c-display-lib.h"

#include "keyboard.h"

void display_keyboard(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void start_keyboard()
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
		
	display_keyboard("Module", 0);
	display_keyboard("Keyboard", 0);
		
	delay(2000);
	return;
}
#endif