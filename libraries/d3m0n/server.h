#pragma execution_character_set("utf-8")

#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include "settings.h"

#include <string.h>
#include <SPI.h>
#include <SD.h>
#include "i2c-display-lib.h"

WebServer server(80);

String IpAddress2String2(const IPAddress& ipAddress)
{
	return String(ipAddress[0]) + String(".") +\
	String(ipAddress[1]) + String(".") +\
	String(ipAddress[2]) + String(".") +\
	String(ipAddress[3])  ; 
}


class d3m0n_server
{
public:
	void start()
	{
		server.addHook([](const String & method, const String & url, WiFiClient * client, WebServer::ContentTypeFunction contentType) {
	      char* website_path = "/website";
	      String path = (String)website_path+url;
	      if(url.startsWith("/api/"))
	      {
	        String filecontent;
	        if(url.startsWith("/api/logs"))
	        {
	          File returnfile = SD.open("log.txt");
	          while(returnfile.available())
	          {
	            filecontent=filecontent+"/"+(String)returnfile.readStringUntil('\r\n');
	          }
	          returnfile.close();
	        }
	        if(url.startsWith("/api/keyboard/getPayload") && method == "GET")
	        {   
	          File returnfile = SD.open("selected_payload.txt");
	          while(returnfile.available())
	          {
	            filecontent=filecontent+(String)returnfile.readStringUntil('.');
	          }
	          returnfile.close();
	        }
	        if(url.startsWith("/api/fb") && method == "GET")
	        {
	        	// soon file browser for debug
	        }
	        if(url.startsWith("/api/keyboard/setPayload") && method == "POST")
	        {
	          if(SD.exists("selected_payload.txt"))
	          {
	          	SD.remove("selected_payload.txt");
	          }

			  		char** temp = Settings.str_split((char*)server.arg("plain").c_str(), "=");

			  		Settings.eventLog((char*)server.arg("plain").c_str());

	          String payloadslot = temp[1]+(String)".";
	          Settings.eventLog("temp[0]: ");
	          Settings.eventLog(temp[0]);
	          Settings.eventLog("payloadslot: ");
	          Settings.eventLog((char*)payloadslot.c_str());


	          File payloadfile = SD.open("selected_payload.txt", FILE_WRITE);
	          payloadfile.println((char*)payloadslot.c_str());
	          payloadfile.close();
	        }
	        server.send(200, "text/plain", (char*)filecontent.c_str());
	      }
	      else
	      {
	        if(method == "GET" && SD.exists((char*)path.c_str()))
	        {
	          if(path.endsWith("/"))
	          {
	            // it's a directory (send path/index.html)
	            path=path+(String)"index.html";
	          }
	          String filecontent;
	          File returnfile = SD.open(path, FILE_READ);
	          while(returnfile.available())
	          {
	            filecontent=filecontent+(String)returnfile.readStringUntil('\r\n');
	          }
	          returnfile.close();
	          server.send(200, "text/html", (char*)filecontent.c_str());
	        }
	        else
	        {
	          server.send(404, "text/plain", "this file don't exists");
	        }
	      }
	      Serial.printf("Request webserver");
	      String logweb = (String)"[website] '"+IpAddress2String2(server.client().remoteIP())+"' requested '"+url+"'";
	      Settings.eventLog((char*)logweb.c_str());
	      return WebServer::CLIENT_REQUEST_CAN_CONTINUE;
	    });
	    
	    server.begin();
	}

	void handle()
	{
		server.handleClient();
	}
};
extern d3m0n_server d3m0n_server;