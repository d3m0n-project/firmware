#ifndef INFRARED_H
#define INFRARED_H

#pragma execution_character_set("utf-8")

#include "i2c-display-lib.h"

#include "infrared.h"
#include "d3m0n.h"
#include "server.h"
#include "settings.h"
#include <IRremote.hpp>
#include <IRSend.hpp>

#define IR_RECEIVE_PIN 14
#define IR_SEND_PIN 15 

//setup buttons
#define button_up 22
#define button_ok 21
#define button_down 20

void save_ir()
{
	lcd_clear();

	Settings.display("Capturing IR... ", 0);
	Settings.display("press OK to stop", 1);
	delay(1000);
	while(true)
	{
		if (IrReceiver.decode())
		{
			Settings.display("IR captured!", 0);
	        IrReceiver.printIRResultShort(&Serial);
	        IrReceiver.printIRSendUsage(&Serial);
	        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
	        	Settings.display("Unknown signal  ", 1);
	            Serial.println("Received noise or an unknown (or not yet enabled) protocol");
	            // We have an unknown protocol here, print more info
	            IrReceiver.printIRResultRawFormatted(&Serial, true);
	            Serial.println(IrReceiver.decodedIRData.command);
	            break;
	        }
	        String protocol = (String)getProtocolString(IrReceiver.decodedIRData.protocol);
	        String command = (String)"0x"+(String)IrReceiver.decodedIRData.command;

	        int tempaddress = IrReceiver.decodedIRData.address;

	        String address = String(tempaddress, HEX);
	        String resultvalue = (String)"Saved as "+Settings.add_file("saved_ir/", "save", "protocol: "+protocol+"\ncommand: "+command+"\naddress: "+address);
	        Settings.display(resultvalue, 1);
	        break;
	    }
	    if(digitalRead(button_ok) == LOW)
	    {
	    	break;
	    }
	}
	IrReceiver.resume();
	delay(2000);
}

void play_ir()
{
	//send lapin vert
	Settings.display("sent! ", 0);
	IrSender.sendNEC(0xEF00, 0x6, 1);
	delay(150);
}

void list_ir()
{
	lcd_clear();
	File saved_ir_path = SD.open("/saved_ir/");
	char** saved_ir = Settings.listdirectory(saved_ir_path);

	int list_menu=0;

	if(sizeof(saved_ir) > 2)
	{
		Settings.display("Saved IR list", 0);
		while(true)
		{
			Settings.display("> "+(String)saved_ir[list_menu], 1, false);
			if(digitalRead(button_up) == LOW  && list_menu != 0)
		    {
		    	list_menu--;
		    }
		    if(digitalRead(button_ok) == LOW && list_menu == sizeof(saved_ir)-3)
		    {
		    	break;
		    }
		    if(digitalRead(button_ok) == LOW)
		    {
		    	lcd_clear;
		    	// display file name
		    	Settings.display((String)saved_ir[list_menu], 0);
		    	// display value
		    	Settings.display(Settings.getSetting("command", saved_ir[list_menu]), 1);
		    	break;

		    }
		    if(digitalRead(button_down) == LOW && list_menu != sizeof(saved_ir)-1)
		    {
		    	list_menu++;
		    }
		}
	}
	else
	{
		Settings.display("No IR code found", 0);
		Settings.display("press OK to stop", 1);
		while(true)
		{
			if(digitalRead(button_ok) == LOW)
		    {
		    	break;
		    }
		}
	}
}
void infrared_exit()
{
	return;
}


void start_infrared()
{
	#define FINISHED 1
	Serial.println("Test readfile: ");
	Serial.println(Settings.readFile("D3M0N_settings.txt"));

	typedef void (*FunctionCallback)();
	FunctionCallback infrared_functions[] = {&play_ir, &save_ir,&list_ir,&infrared_exit};
	char* infrared[] = {"> play          ","> save          ", "> list          ","> exit          "};
	
	lcd_clear();
	
	IrReceiver.begin(IR_RECEIVE_PIN, false);
	IrSender.begin(IR_SEND_PIN);

	int infrared_menu=0;


	while(true)
	{
		Settings.display("Infrared", 0, false);
		Settings.display(infrared[infrared_menu], 1, false);
		if(digitalRead(button_up) == LOW && infrared_menu != 0)
		{
			infrared_menu--;
		}
		if(digitalRead(button_down) == LOW && infrared_menu != Settings.getLength(infrared)-3)
		{
			infrared_menu++;
		}
		if(digitalRead(button_ok) == LOW)
		{
			Serial.print("menu value: ");
			Serial.println(infrared_menu);
			Serial.print("total value: ");
			Serial.println(Settings.getLength(infrared));
			if(infrared_menu == Settings.getLength(infrared)-3)
			{
				break;
			}
			infrared_functions[infrared_menu]();
		}
		d3m0n_server.handle();
	}
	#define FINISHED 0
}
#endif