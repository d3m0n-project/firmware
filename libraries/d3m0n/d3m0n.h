#ifndef D3M0N_H
#define D3M0N_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "d3m0n.h"
#include <string.h>
#include "i2c-display-lib.h"


//modules include
#include "keyboard/keyboard.h"
#include "infrared/infrared.h"
#include "rfid/rfid.h"
#include "wifi/wifi.h"
#include "options/options.h"

// Functions

// function type
typedef void (*FunctionCallback)();
FunctionCallback modulescall[] = {
	&start_rfid, 
	&start_infrared,
	&start_wifi,
	&start_keyboard,
	&start_options,
};

const char* modules[] = {
	"RFID",
	"INFRARED", 
	"WIFI",
	"KEYBOARD",
	"OPTIONS"
};

void display2(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
};

void d3m0n_begin()
{
	//setup buttons
	int button_up = 20;
	int button_ok = 19;
	int button_down = 18;
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
  
	pinMode(button_up, INPUT_PULLUP); 
	pinMode(button_ok, INPUT_PULLUP); 
	pinMode(button_down, INPUT_PULLUP);
	
	if(sizeof(modules) > 0)
	{
		lcd_clear();
		display2(strcat("> ",modules[0]), 0);
		if (sizeof(modules) > 1)
		{
			display2(strcat("  ",modules[1]), 1);
		}
		else
		{
			display2("                ", 1);
		}
		int menu=0;
		bool Firstline=true;
		while(true)
		{
			if(digitalRead(button_down) == LOW && menu+1!=sizeof(modules))
			{
				menu=menu+1;
				
				if ((menu % 2) == 0)
				{
					//pair
					Firstline=true;
					// print("menu: "+str(menu)+" is pair")
				}
				else
				{
					if ((menu % 2) == 1)
					{
						//impair
						Firstline=false;
						// print("menu: "+str(menu)+" is impair")
					}
				}
				
				
				//print("menu: "+str(menu)+"/total: "+str(sizeof(modules)))
				//print("ISfirst?: "+str(Firstline))
				
				
				lcd_clear();
				if(Firstline)
				{
					display2(strcat("> ",modules[menu]), 0);
					if(menu+1==sizeof(modules))
					{
						display2("                ", 1);
					}
					else
					{
						display2(strcat("  ",modules[menu+1]), 1);
					}
					Firstline=false;
				}
				else
				{
					display2(strcat("  ",modules[menu-1]), 0);
					if(menu==sizeof(modules))
					{
						display2(strcat("> ",modules[menu-1]), 0);
						display2("                ", 1);
					}
					else
					{
						display2(strcat("> ",modules[menu]), 1);
					}
					Firstline=true;
				}
				while(digitalRead(button_down) == LOW)
				{
					delay(0);
				}
			}
			if(digitalRead(button_up) == LOW)
			{
				if(menu==0)
				{
					Firstline=true;
				}
				else
				{
					menu=menu-1;
				}
				
				//print("menu: "+str(menu)+"/total: "+str(sizeof(modules)))
				//print("ISfirst?: "+str(Firstline))
				
				lcd_clear();
				if(Firstline)
				{
					display2(strcat("> ",modules[menu]), 0);
					if(menu+2>sizeof(modules))
					{
						display2("                ", 1);
					}
					else
					{
						display2(strcat("  ",modules[menu+1]), 1);
					}
					Firstline=false;
				}
				else
				{
					display2(strcat("  ",modules[menu-1]), 0);
					if(menu>sizeof(modules))
					{
						display2("                ", 1);
					}
					else
					{
						display2(strcat("> ",modules[menu]), 1);
					}
					Firstline=true;
				}
				while (digitalRead(button_up) == LOW)
				{
					delay(0);
				}
			}
			if(digitalRead(button_ok) == LOW)
			{
				modulescall[menu];
			}
		}
	}
};
#endif
