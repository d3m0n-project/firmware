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
#include "radio/radio.h"
#include "custom/custom1.h"
#include "custom/custom2.h"
#include "custom/custom3.h"

// Vars
//setup buttons
#define button_up 20
#define button_ok 19
#define button_down 18

// function type
typedef void (*FunctionCallback)();
FunctionCallback modulescall[] = {&start_rfid, &start_infrared,&start_wifi,&start_keyboard,&start_options, &start_options};

char* modules1[] = {"> RFID","> INFRARED", "> WIFI","> KEYBOARD","> RADIO","> OPTIONS"};
char* modules2[] = {"  RFID","  INFRARED", "  WIFI","  KEYBOARD","  RADIO","  OPTIONS"};

void display2(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void d3m0n_begin()
{	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
  
	pinMode(button_up, INPUT_PULLUP); 
	pinMode(button_ok, INPUT_PULLUP); 
	pinMode(button_down, INPUT_PULLUP);
	
	if(sizeof(modules1) > 0)
	{
		lcd_clear();
		display2(modules1[0], 0);
		if (sizeof(modules1) > 1)
		{
			display2(modules2[1], 1);
		}
		else
		{
			display2("                ", 1);
		}
		int menu=0;
		bool Firstline=true;
		while(true)
		{
			if(digitalRead(button_down) == LOW && menu+1!=sizeof(modules1))
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
				
				
				//print("menu: "+str(menu)+"/total: "+str(sizeof(modules1)))
				//print("ISfirst?: "+str(Firstline))
				
				
				lcd_clear();
				if(Firstline)
				{
					display2(modules1[menu], 0);
					// if(menu+1==sizeof(modules1))
					if(menu==sizeof(modules1))
					{
						display2("                ", 1);
					}
					else
					{
						// display2(modules2[menu+1], 1);
						display2(modules2[menu+1], 1);
					}
					Firstline=false;
				}
				else
				{
					display2(modules2[menu-1], 0);
					if(menu==sizeof(modules1))
					{
						display2(modules1[menu-1], 0);
						display2("                ", 1);
						// display2("                ", 1);
					}
					else
					{
						display2(modules1[menu], 1);
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
				
				//print("menu: "+str(menu)+"/total: "+str(sizeof(modules1)))
				//print("ISfirst?: "+str(Firstline))
				
				lcd_clear();
				if(Firstline)
				{
					display2(modules1[menu], 0);
					if(menu+2>sizeof(modules1))
					{
						display2("                ", 1);
					}
					else
					{
						display2(modules2[menu+1], 1);
					}
					Firstline=false;
				}
				else
				{
					display2(modules2[menu-1], 0);
					if(menu>sizeof(modules1))
					{
						display2("                ", 1);
					}
					else
					{
						display2(modules1[menu], 1);
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
				modulescall[menu]();
				delay(1000);
				break;
			}
		}
	}
}
#endif
