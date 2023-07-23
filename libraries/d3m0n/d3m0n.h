#ifndef D3M0N_H
#define D3M0N_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "d3m0n.h"
#include "settings.h"
#include "server.h"
#include <string.h>
#include <SPI.h>
#include <SD.h>
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
#define button_up 22
#define button_ok 21
#define button_down 20
const int _MISO_pin = 0; //rx
const int _MOSI_pin = 7; //tx
const int _CS_pin = 5;   //csn
const int _SCK_pin = 6;  // SCK

#define FINISHED 0

// function type
typedef void (*FunctionCallback)();
FunctionCallback modulescall[] = {&start_rfid, &start_infrared,&start_wifi,&start_keyboard,&start_options, &start_options};

char* modules1[] = {"> RFID","> INFRARED", "> WIFI","> KEYBOARD","> RADIO","> OPTIONS"};
char* modules2[] = {"  RFID","  INFRARED", "  WIFI","  KEYBOARD","  RADIO","  OPTIONS"};


void d3m0n_begin()
{	
	SPI.setRX(_MISO_pin);
	SPI.setTX(_MOSI_pin);
	SPI.setSCK(_SCK_pin);
	  
	if (!SD.begin(_CS_pin)) {
	  Settings.display("No SD card found", 0);
	  return;
	}
	d3m0n_server.start();

	if(Settings.getLength(modules1) > 0)
	{
		lcd_clear();
		Settings.display(modules1[0], 0);
		if (Settings.getLength(modules1) > 1)
		{
			Settings.display(modules2[1], 1);
		}
		else
		{
			Settings.display("                ", 1);
		}
		int menu=0;
		bool Firstline=true;
		while(true)
		{
			if(digitalRead(button_down) == LOW && menu!=Settings.getLength(modules1)-3 && FINISHED == 0)
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
					Settings.display(modules1[menu], 0);
					// if(menu+1==sizeof(modules1))
					if(menu==Settings.getLength(modules1))
					{
						Settings.display("                ", 1);
					}
					else
					{
						// Settings.display(modules2[menu+1], 1);
						Settings.display(modules2[menu+1], 1);
					}
					Firstline=false;
				}
				else
				{
					Settings.display(modules2[menu-1], 0);
					if(menu==Settings.getLength(modules1))
					{
						Settings.display(modules1[menu-1], 0);
						Settings.display("                ", 1);
						// Settings.display("                ", 1);
					}
					else
					{
						Settings.display(modules1[menu], 1);
					}
					Firstline=true;
				}
				while(digitalRead(button_down) == LOW)
				{
					delay(0);
				}
			}
			if(digitalRead(button_up) == LOW && FINISHED == 0 && menu != 0)
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
					Settings.display(modules1[menu], 0);
					if(menu+2>Settings.getLength(modules1))
					{
						Settings.display("                ", 1);
					}
					else
					{
						Settings.display(modules2[menu+1], 1);
					}
					Firstline=false;
				}
				else
				{
					Settings.display(modules2[menu-1], 0);
					if(menu>Settings.getLength(modules1))
					{
						Settings.display("                ", 1);
					}
					else
					{
						Settings.display(modules1[menu], 1);
					}
					Firstline=true;
				}
				while (digitalRead(button_up) == LOW && FINISHED == 0)
				{
					delay(0);
				}
			}
			if(digitalRead(button_ok) == LOW && FINISHED == 0)
			{
				#define FINISHED 1
				modulescall[menu]();
			}
			d3m0n_server.handle();
		}
	}
}
#endif
