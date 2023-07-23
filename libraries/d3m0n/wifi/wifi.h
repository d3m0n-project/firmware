#ifndef wifi_H
#define wifi_H


#include "i2c-display-lib.h"

#include "wifi.h"
#include <WiFi.h>

//setup buttons
#define button_up 22
#define button_ok 21
#define button_down 20

void wifi_scan()
{
	Serial.println("Started wifi scan");
	Settings.display("Scanning APs...", 0);
	
	int n = WiFi.scanNetworks();
	Settings.display("Scan done", 1);
	if (n == 0) {
		Settings.display("no APs found", 1);
	} else {
		char* networks[50] = {};
		for (int i = 0; i < n; ++i) {
			// Print SSID and RSSI for each network found
			char* network_name = (char*)WiFi.SSID(i);
			char* networks[i]={"> "};
			strcat(networks[i], network_name);
		}
		int wifi_scan_menu=0;
		
		Settings.display(networks[0], 1);
		
		while(digitalRead(button_up) != LOW && wifi_scan_menu != 0)
		{
			if (digitalRead(button_down) == LOW) {
				wifi_scan_menu=wifi_scan_menu+1;
				Settings.display(networks[wifi_scan_menu], 1);
				while(digitalRead(button_down) == LOW){
				  //do nothin
				}
			}
			if (digitalRead(button_up) == LOW) {
				wifi_scan_menu=wifi_scan_menu-1;
				Settings.display(networks[wifi_scan_menu], 1);
				while(digitalRead(button_up) == LOW){
				  //do nothin
				}
			}
			if (digitalRead(button_ok) == LOW) {
				break;
			}
		}
	}
}

void wifi_keylogger()
{
	#define FINISHED 0
}


void wifi_menu(int menu)
{
	switch(menu)
	{
		case 0:
			Settings.display("> Scan APs      ", 1);
			break;
		case 1:
			Settings.display("> Guess password", 1);
			break;
		case 2:
			Settings.display("> EXIT", 1);
			break;
	}
}

void select_wifi_menu(int menu)
{
	switch(menu)
	{
		case 0:
			wifi_scan();
			break;
		case 1:
			wifi_keylogger();
			break;
		case 2:
			break;
	}
}

void start_wifi()
{
	#define FINISHED 1
	
	//setup wifi
	//WiFi.mode(WIFI_STA);
	//WiFi.disconnect();
	delay(100);
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
		
	lcd_clear();
	
	Settings.display("Wifi module     ", 0);
	
	
	int wifi_menu_value=0;
	wifi_menu(wifi_menu_value);

	
	//digitalRead(button_up) != LOW && wifi_menu_value != 0
	while(true)
	{
		if (digitalRead(button_down) == LOW) {
			wifi_menu_value=wifi_menu_value+1;
			wifi_menu(wifi_menu_value);
			while(digitalRead(button_down) == LOW){
			  //do nothin
			}
		}
		if (digitalRead(button_up) == LOW) {
			wifi_menu_value=wifi_menu_value-1;
			wifi_menu(wifi_menu_value);
			while(digitalRead(button_up) == LOW){
			  //do nothin
			}
		}
		if (digitalRead(button_ok) == LOW) {
			Serial.println("Started wifi scan button");
			select_wifi_menu(wifi_menu_value);

			while(digitalRead(button_ok) == LOW){
			  //do nothin
			}
		}
	}
	Serial.println("finished wifi");
	#define FINISHED 0
}
#endif