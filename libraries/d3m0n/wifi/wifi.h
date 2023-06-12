#ifndef wifi_H
#define wifi_H


#include "i2c-display-lib.h"

#include "wifi.h"
#include <WiFi.h>

//setup buttons
#define button_up 20
#define button_ok 19
#define button_down 18

void display_wifi(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}

void wifi_scan()
{
	display_wifi("Scanning APs...", 0);
	
	int n = WiFi.scanNetworks();
	display_wifi("Scan done", 1);
	if (n == 0) {
		display_wifi("no APs found", 1);
	} else {
		char* networks[50] = {};
		for (int i = 0; i < n; ++i) {
			// Print SSID and RSSI for each network found
			char* network_name = (char*)WiFi.SSID(i);
			char* networks[i]={"> "};
			strcat(networks[i], network_name);
		}
		int wifi_scan_menu=0;
		
		display_wifi(networks[0], 1);
		
		while(digitalRead(button_up) != LOW && wifi_scan_menu != 0)
		{
			if (digitalRead(button_down) == LOW) {
				wifi_scan_menu=wifi_scan_menu+1;
				display_wifi(networks[wifi_scan_menu], 1);
				while(digitalRead(button_down) == LOW){
				  //do nothin
				}
			}
			if (digitalRead(button_up) == LOW) {
				wifi_scan_menu=wifi_scan_menu-1;
				display_wifi(networks[wifi_scan_menu], 1);
				while(digitalRead(button_up) == LOW){
				  //do nothin
				}
			}
			if (digitalRead(button_ok) == LOW) {
				break;
			}
		}
		return;
	}
}

void wifi_keylogger()
{
	//
}


void wifi_menu(int menu)
{
	switch(menu)
	{
		case 0:
			display_wifi("> Scan APs      ", 1);
			break;
		case 1:
			display_wifi("> Guess password", 1);
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
	}
}

void start_wifi()
{
	pinMode(button_up, INPUT_PULLUP); //up
	pinMode(button_ok, INPUT_PULLUP); //ok
	pinMode(button_down, INPUT_PULLUP); //down
		
	//setup wifi
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	
	delay(100);
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
		
	lcd_clear();
	
	display_wifi("Wifi module     ", 0);
	
	
	int wifi_menu_value=0;
	wifi_menu(wifi_menu_value);
	
	
	while(digitalRead(button_up) != LOW && wifi_menu_value != 0)
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
			select_wifi_menu(wifi_menu_value);
			while(digitalRead(button_ok) == LOW){
			  //do nothin
			}
		}
	}
	
	return;
}
#endif