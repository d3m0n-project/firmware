#pragma once
#pragma execution_character_set("utf-8")

#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings.h"
#include <defaults.h>
#include <WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <assert.h>

#include <iostream>

#include <time.h>


#include <SPI.h>
#include <SD.h>

class SettingsClass {
public:
	char* readFile(char* path)
	{
		String filecontent;
		File file = SD.open(path, FILE_READ);
		char* bytes;
		int i=0;
		while(file.available())
		{
			Serial.println(file.read());
			bytes[i]=file.read();
			i++;
			filecontent=filecontent+(String)file.readStringUntil('\n');
			// bytes[i]="";
			// i++;
			// add file.read() to bytes
		}
		Serial.print("filecontent: ");
		Serial.println(filecontent);



		return bytes;
	}

	int getLength(char** list)
	{
		int total = 0;
		for(int i=0;list[i]!=NULL;i++)
		{
		  total++;
		}
		return total;
	}
	void eventLog(char* event)
	{
	  time_t now;
	  char buff[80];

	  time(&now);
	  strftime(buff, sizeof(buff), "%c", localtime(&now));

	  
	  File logfile = SD.open("log.txt", FILE_WRITE);
	  String date=(String)buff;
	  String temp = "["+date+"] "+(String)event;
	  logfile.println((char*)temp.c_str());
	  logfile.close();
	}
	void display(String text, int line, bool enablelog=true)
	{
		if(enablelog)
		{
			eventLog((char*)text.c_str());
		}
  		lcd_setCursor(line, 0);
  		lcd_print(text.c_str());
	}
	// return filename
	String add_file(String path, String name, String content)
	{
		int i=0;
		char num[200];
		while(true)
		{
			sprintf(num, "%d", i);
			Serial.print("num value: ");
			Serial.println((String)num);
			String temp=path+name+(String)num;
			char* filepath=(char*)temp.c_str();
			Serial.print("filepath value: ");
			Serial.println(filepath);
			if(!SD.exists(filepath))
			{
				File file = SD.open(filepath, FILE_WRITE);
				file.println((char*)content.c_str());
				break;
			}
			else
			{
				i++;
			}
		}
		return name+(String)num;
	}


	char* getSetting(char* name, const char *file="D3M0N_settings.txt")
	{
		char* test="this is a sample: thisisvalue";
		char** tempvar=str_split(test, ":");


		Serial.print("test: ");
		Serial.print(tempvar[0]);
		Serial.print(" is equal to ");
		Serial.println(tempvar[1]);




		if(checkSettingExists() == 1)
		{
			eventLog("[Settings.h] no SD card found");
			return "no SD card found";
		}

		String temp1 = (String)"[Settings.h] name='"+(String)name+"'";
		eventLog((char*)temp1.c_str());

		char** foundname;

		File settings = SD.open(file, FILE_READ);
		char* returnValue = "not found";
		while (settings.available()) 
		{
		    const char* line = settings.readStringUntil('\n').c_str();

		    Serial.print("line: '");
		    Serial.print(line);
		    Serial.println("'");

		    if(strstr(line, ": ") != NULL)
		    {
		    	eventLog((char*)line);

		    	foundname = str_split((char*)line, ":");

		    	Serial.print("foundname0: ");
		    	Serial.println(foundname[0]);
		    	Serial.print("foundname1: ");
		    	Serial.println(foundname[1]);
		    	Serial.print("foundname2: ");
		    	Serial.println(foundname[2]);

		    	char* value=(char*)foundname[1];
		    	if (value[0] == ' ') 
		    	{
		    		memmove(value, value+1, strlen(value));
		    	}
		    	temp1 = (String)"[Settings.h] foundname='"+(String)foundname[0]+(String)"' is equal to '"+(String)value+(String)"'";
				eventLog((char*)temp1.c_str());
				if((String)name==(String)foundname[0])
				{
					settings.close();
					returnValue = (char*)value;
				}
				else
				{
					temp1 = (String)"[Settings.h] foundname='"+(String)foundname[0]+(String)"' != '"+(String)name+(String)"'";
					eventLog((char*)temp1.c_str());
				}
		    }
		}
		settings.close();
		return returnValue;
	}

	void addSetting(char* name, char* value, const char *file="D3M0N_settings.txt")
	{
		if(checkSettingExists() == 1)
		{
			return;
		}



		String newline=(String)name+(String)": "+(String)value;

		File settings = SD.open(file, FILE_READ);
		settings.println(newline.c_str());
		settings.close();
	}
	// char** str_split( char* str, const char* delim)
	// {
	//     char* token = strtok(str, delim);
   
	//     char** ret;
	//     int i=0;
	//     while (token != NULL)
	//     {
	//         ret[i] = token;
	//         token = strtok(NULL, delim);
	//         i++;
	//     }

	//     return ret;
	// }
	char** str_split(char *line, const char* delimiter) 
	{
	    char *tmp;
	    char** vartoreturn;

	    tmp = strtok(line, delimiter);
	    if (tmp == NULL)
	    {
	    	return vartoreturn;
	    }

	    vartoreturn[0] = tmp;

	    for (int i=1;1==1;i++) 
	    {
	        tmp = strtok(NULL, delimiter);
	        if (tmp == NULL)
	        {
	            break;
	        }
	        vartoreturn[i] = tmp;
	    }
	    return vartoreturn;
	}
	char** listdirectory(File dir) 
	{
		char** directory_list;
		int i=0;
	  	while (true) {

	   	 	File entry =  dir.openNextFile();
	    	if (! entry) {
	      		Serial.println("no listDir file");
	      		break;
	    	}



	    	Serial.print(entry.name());
	    	if (!entry.isDirectory()) {
	      		directory_list[i] = (char*)entry.name();
	      		i++;
	    	}
	    	entry.close();
	 	}
	 	return directory_list;
	}
	char** splitFile(char* path, char delim)
	{
		File file = SD.open(path, FILE_READ);
		char* totalfile;
		uint32_t pos = (uint32_t)0;
		while (file.available()) 
		{
			file.seek(pos);
		    const char* line = file.readStringUntil(delim).c_str();
		    pos = file.position();


		}
		char** test;
		return test;
	}
private:
	int checkSettingExists()
	{
		Serial.begin(9600);

		SPI.setRX(0);
	  	SPI.setTX(7);
	  	SPI.setSCK(6);
	  
	 	if (!SD.begin(5)) {
	    	return 1;
	  	}

	  	if (!SD.exists("D3M0N_settings.txt")) {
	    	File settings = SD.open("D3M0N_settings.txt", FILE_WRITE);
	  		settings.println("ap_ssid: D3M0N-X1");
	  		settings.println("ap_password: changeme");
	  		settings.println("password_list: passwords.txt");
	  		settings.println("startup_time: 1");
	  		settings.println("web_address: 192.168.1.4");
	  		settings.println("web_port: 80");
	  		settings.println("web_path: /webserver");
	  		settings.close();
	  		if (!SD.exists("/webserver/index.html")) {
	  			SD.mkdir("/webserver");
	  			//File file = SD.open("/webserver/index.html", FILE_WRITE);
	  			//file.println(settings_defaults.webserver.index);
	  			//file.close;
	  		}
	  	}
	  	if (!SD.exists("/saved_ir")) {
	  		SD.mkdir("/saved_ir");
	  	}
	  	return 0;
	}
};
extern SettingsClass Settings;
#endif