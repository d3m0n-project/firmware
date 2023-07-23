#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "i2c-display-lib.h"

#include "keyboard.h"
#include "Keyboard.h"

//setup buttons
#define button_up 22
#define button_ok 21
#define button_down 20

void display_keyboard(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void script_to_key(char* expression, char* after="")
{
    if(expression == "STRING")    	Keyboard.print(after);
    if(expression == "GUI r")     	Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r');
    if(expression == "GUI")     	Keyboard.press(KEY_LEFT_GUI);
    if(expression == "DELAY")  		delay((int)after/1000);
    if(expression == "ENTER") 		Keyboard.press(KEY_RETURN);
    if(expression == "CTRL") 		Keyboard.press(KEY_LEFT_CTRL);
    if(expression == "ALT") 		Keyboard.press(KEY_LEFT_ALT); 
    if(expression == "CTRL-SHIFT") 	Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(KEY_RIGHT_SHIFT); if(after!="") Keyboard.print(after);
    if(expression == "CTRL-ALT") 	Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(KEY_LEFT_ALT); if(after!="") Keyboard.print(after);
    if(expression == "ESC") 		Keyboard.press(KEY_ESC); 
	if(expression == "EXIT")       	return; 
	if(expression == "TAB")        	Keyboard.press(KEY_TAB);
	if(expression == "CTRL-TAB")    Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(KEY_TAB);
	if(expression == "ALT-TAB")   	Keyboard.press(KEY_LEFT_ALT); Keyboard.press(KEY_TAB);
	if(expression == "RIGHT")   	Keyboard.press(KEY_RIGHT_ARROW);
	if(expression == "LEFT")   	 	Keyboard.press(KEY_LEFT_ARROW);
	if(expression == "UP") 			Keyboard.press(KEY_UP_ARROW);
	if(expression == "DOWN")   	 	Keyboard.press(KEY_DOWN_ARROW);
	if(expression == "REM")   	 	// nothin' it's a comment
	
	Keyboard.releaseAll();
}

void keyboard_badUSB(char* payload)
{
	while(true)
	{
		if (digitalRead(button_ok) == LOW) 
		{
			//Send the message
			Keyboard.print("Hello");
			break;
		}
	}
}

void keyboard_keylogger()
{
	while(true)
	{
		if (digitalRead(button_ok) == LOW) 
		{
			//Send the message
			Keyboard.print("Hello");
		}
	}
}


void keyboard_menu(int menu)
{
	switch(menu)
	{
		case 0:
			display_keyboard("> badUSB", 1);
			break;
		case 1:
			display_keyboard("> Keylogger", 1);
			break;
	}
}

void select_keyboard_menu(int menu)
{
	switch(menu)
	{
		case 0:
			keyboard_badUSB("edit me later");
			break;
		case 1:
			keyboard_keylogger();
			break;
	}
}

void start_keyboard()
{
	#define FINISHED 1
	pinMode(button_up, INPUT_PULLUP); //up
	pinMode(button_ok, INPUT_PULLUP); //ok
	pinMode(button_down, INPUT_PULLUP); //down
		
	//setup keyboard
	Keyboard.begin();
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
		
	lcd_clear();
	
	display_keyboard("Keyboard module ", 0);
	
	
	int keyboard_menu_value=0;
	keyboard_menu(keyboard_menu_value);
	
	
	while(digitalRead(button_up) != LOW && keyboard_menu_value != 0)
	{
		if (digitalRead(button_down) == LOW) {
			keyboard_menu_value=keyboard_menu_value+1;
			keyboard_menu(keyboard_menu_value);
			while(digitalRead(button_down) == LOW){
			  //do nothin
			}
		}
		if (digitalRead(button_up) == LOW) {
			keyboard_menu_value=keyboard_menu_value-1;
			keyboard_menu(keyboard_menu_value);
			while(digitalRead(button_up) == LOW){
			  //do nothin
			}
		}
		if (digitalRead(button_ok) == LOW) {
			select_keyboard_menu(keyboard_menu_value);
			while(digitalRead(button_ok) == LOW){
			  //do nothin
			}
		}
	}
	
	#define FINISHED 0
}
#endif