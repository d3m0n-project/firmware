#pragma execution_character_set("utf-8")

#include "Keyboard.h"
#include <stdio.h>
#include "i2c-display-lib.h"
#include "d3m0n.h"
#include "settings.h"
#include <string.h>
#include <WiFi.h>
#include <time.h>

const int _MISO = 0; //rx
const int _MOSI = 7; //tx
const int _CS = 5;   //csn
const int _SCK = 6;  // SCK


#include <SPI.h>
#include <SD.h>



//setup buttons
#define button_up 22
#define button_ok 21
#define button_down 20

char* ssid = "D3M0N-X1";
char* pass = "changeme";
String mac_address;
String ip_address;

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

void setup() {
  //Serial.begin(9600);
  //Serial.print("d3m0n serial");
  char** test = Settings.splitFile("D3M0N_settings.txt", '\n');
  Serial.print("test: ");
  for(int i=0; i < sizeof(test); i++)
  {
    Serial.print(test[i]);
    Serial.print(";");
  }
  
  Serial.println("");
  
  //setup lcd display
  lcd_setAddr(0x3f);
  lcd_init(16, 17);
  
  SPI.setRX(_MISO);
  SPI.setTX(_MOSI);
  SPI.setSCK(_SCK);
  
  if (!SD.begin(_CS)) {
    Settings.display("No SD card found", 0);
    while(true)
    {
       if (SD.begin(_CS)) {
        break;
      }
    }
  }

  if (SD.exists("log.txt")) {
    SD.remove("log.txt");
  }
  
  File logfile = SD.open("log.txt", FILE_WRITE);
  logfile.println("                            ██████╗ ██████╗ ███╗   ███╗ ██████╗ ███╗   ██╗                            ");
  logfile.println("                            ██╔══██╗╚════██╗████╗ ████║██╔═████╗████╗  ██║                            ");
  logfile.println("                            ██║  ██║ █████╔╝██╔████╔██║██║██╔██║██╔██╗ ██║                            ");
  logfile.println("                            ██║  ██║ ╚═══██╗██║╚██╔╝██║████╔╝██║██║╚██╗██║                            ");
  logfile.println("                            ██████╔╝██████╔╝██║ ╚═╝ ██║╚██████╔╝██║ ╚████║                            ");
  logfile.println("                            ╚═════╝ ╚═════╝ ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝");
  logfile.println("                                         https://github.com/d3m0n-project");
  logfile.println("                                           made with love by 4re5 group");
  logfile.println("");
  logfile.close();

  
  Settings.eventLog("Started successfully");

  ssid = Settings.getSetting("ap_ssid");
  //pass = Settings.getSetting("ap_password");
  String temp2 = (String)"PASSWORD: "+(String)pass;
  String temp1 = (String)"SSID: "+(String)ssid;
  Settings.eventLog((char*)temp1.c_str());
  Settings.eventLog((char*)temp2.c_str());
  

  //setup ap
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(ssid, pass);
  if(result){
    ip_address=IpAddress2String(WiFi.localIP());
    Settings.display("IP address:", 0);
    Settings.display(ip_address, 1);
    mac_address = WiFi.softAPmacAddress().c_str();
  }else{
    Settings.display("AP setup Failed", 0);
    return;
  }
  
  pinMode(button_up, INPUT_PULLUP); //up
  pinMode(button_ok, INPUT_PULLUP); //ok
  pinMode(button_down, INPUT_PULLUP); //down

  delay(1000);
  Settings.display("    D3M0N-X1    ", 0);
  Settings.display("  by 4RE5 team  ", 1);
  char* startup_time = Settings.getSetting("startup_time"); 
  float ftemp = atof(startup_time) * 1000;
  delay(ftemp);

  d3m0n_begin();
}

void loop()
{
  // nothing
}
